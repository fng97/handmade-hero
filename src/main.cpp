#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <cstdlib>
#include <iostream>
#include <stdio.h>

constexpr int DEFAULT_WINDOW_WIDTH = 640;
constexpr int DEFAULT_WINDOW_HEIGHT = 480;

int main() {
  int last_width = DEFAULT_WINDOW_WIDTH;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return -1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Handmade Hero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

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

  SDL_Texture *texture = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
      DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

  void *pixels = malloc(DEFAULT_WINDOW_WIDTH * DEFAULT_WINDOW_HEIGHT * 4);

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

        if (pixels) {
          free(pixels);
        }

        if (texture) {
          SDL_DestroyTexture(texture);
        }

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    event.window.data1, event.window.data2);

        last_width = event.window.data2;
        pixels = malloc(event.window.data1 * event.window.data2 * 4);

        break;

      case SDL_WINDOWEVENT_FOCUS_GAINED:
        printf("SDL_WINDOWEVENT_FOCUS_GAINED\n");
        break;

      case SDL_WINDOWEVENT_EXPOSED:
        SDL_UpdateTexture(texture, 0, pixels, last_width * 4);
        SDL_RenderCopy(renderer, texture, 0, 0);
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
