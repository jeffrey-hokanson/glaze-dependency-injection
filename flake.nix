{
  description = "Development environment with Python and Clang";

  inputs = {
    # nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # Python
            python3
            python3Packages.pip
            python3Packages.virtualenv

            # Dependencies
        glaze
            # Build tools
	    cmake
	    gnumake
	    llvmPackages_20.clang
	    llvmPackages_20.libcxx
	    ninja

	    # Other
	    git
          ];

          shellHook = ''
            echo "Development environment loaded!"
            echo "Python: $(python --version)"
            echo "Clang: $(clang --version | head -n1)"
            echo ""
            echo "Available packages:"
            echo "  - Python 3 with pip and virtualenv"
            echo "  - Clang compiler"
            echo "  - Ceres Solver (non-linear optimization library)"
            echo "  - Eigen, glog, gflags (Ceres dependencies)"
          '';
        };
      }
    );
}
