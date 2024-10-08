{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShell = pkgs.clangStdenv.mkDerivation {
          name = "Handmade Hero Dev Shell";
          nativeBuildInputs = with pkgs; [ 
            cmake
            ninja
            ccache
            SDL2
          ];
        };
      }
    );
}
