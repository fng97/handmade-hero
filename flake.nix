{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        # llvm = pkgs.pkgsLLVM.llvmPackages_latest;
      in
      {
        devShell = pkgs.clangStdenv.mkDerivation {
          name = "Handmade Hero Dev Shell";
          nativeBuildInputs = with pkgs; [ 
            cmake
            ninja
            ccache
            SDL2
            # llvm.bintools
          ]
          # ++ lib.optionals pkgs.stdenv.isLinux [ llvm.lld ]
          ;

          # LD_LIBRARY_PATH = nixpkgs.lib.strings.makeLibraryPath [ llvm.libcxx ];
        };
      }
    );
}
