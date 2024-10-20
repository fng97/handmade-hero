#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <iostream>
#include <stdio.h>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return -1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Handmade Hero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

  if (!window) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return -1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    std::cerr << "Failed to create renderer. SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  SDL_Event event;
  bool quit = false;

  while (quit == false) {
    if (!SDL_PollEvent(&event)) {
      continue;
    }

    switch (event.type) {
    case SDL_QUIT:
      std::cout << "SDL_QUIT" << std::endl;
      quit = true;
      break;

    case SDL_WINDOWEVENT:
      switch (event.window.event) {
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        printf("SDL_WINDOWEVENT_SIZE_CHANGED: %d x %d\n", event.window.data1,
               event.window.data2);
        break;

      case SDL_WINDOWEVENT_FOCUS_GAINED:
        printf("SDL_WINDOWEVENT_FOCUS_GAINED\n");
        break;

      case SDL_WINDOWEVENT_EXPOSED:
        static bool is_white = true;

        if (is_white == true) {
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          is_white = false;
        } else {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          is_white = true;
        }

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        break;

      default:
        // std::cout << "Unhandled SDL_WINDOWEVENT event: " <<
        // event.window.event << std::endl;
        break;
      }

    default:
      // std::cout << "Unhandled SDL_Event: " << event.type << std::endl;
      break;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}
