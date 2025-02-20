{
  description = "Fast and flexible implementation of Rigid Body Dynamics algorithms and their analytical derivatives.";

  inputs = {
    coal.url = "github:nim65s/coal/only-py";
    flake-parts.follows = "coal/flake-parts";
    nixpkgs.follows = "coal/nixpkgs";
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = inputs.nixpkgs.lib.systems.flakeExposed;
      perSystem =
        {
          inputs',
          pkgs,
          self',
          system,
          ...
        }:
        {
          _module.args.pkgs = import inputs.nixpkgs {
            inherit system;
            overlays = [
              (final: prev: {
                inherit (self'.packages) coal;
                pythonPackagesExtensions = prev.pythonPackagesExtensions ++ [
                  (python-final: python-prev: {
                    coal = python-final.toPythonModule (
                      self'.packages.py-coal.override {
                        python3Packages = python-final;
                      }
                    );
                  })
                ];
              })
            ];
          };
          apps.default = {
            type = "app";
            program = pkgs.python3.withPackages (_: [ self'.packages.default ]);
          };
          devShells.default =
            with pkgs;
            mkShell {
              inputsFrom = [ self'.packages.pinocchio ];
              packages =
                let
                  py = p: [
                    p.coal
                    p.boot
                    p.eigenpy
                    p.numpy
                    p.scipy
                  ];
                in
                [
                  (python312.withPackages py)
                  (python313.withPackages py)
                ];
            };
          packages = {
            inherit (inputs'.coal.packages)
              coal
              py-coal
              py312-coal
              py313-coal
              ;
            default = self'.packages.pinocchio;
            pinocchio = pkgs.pinocchio.overrideAttrs (super: {
              src = pkgs.lib.fileset.toSource {
                root = ./.;
                fileset = pkgs.lib.fileset.unions [
                  ./benchmark
                  ./bindings
                  ./CMakeLists.txt
                  ./doc
                  ./examples
                  ./include
                  ./models
                  ./package.xml
                  ./sources.cmake
                  ./src
                  ./unittest
                  ./utils
                ];
              };
              cmakeFlags = super.cmakeFlags ++ [
                "-DCOAL_DISABLE_HPP_FCL_WARNINGS=ON"
              ];
            });
            py-pinocchio =
              (self'.packages.pinocchio.override {
                inherit (pkgs) python3Packages;
                pythonSupport = true;
              }).overrideAttrs
                (super: {
                  cmakeFlags = super.cmakeFlags ++ [
                    "-DBUILD_ONLY_PYTHON_INTERFACE=ON"
                  ];
                  propagatedBuildInputs = super.propagatedBuildInputs ++ [
                    self'.packages.pinocchio
                  ];
                });
            py312-pinocchio = self'.packages.py-pinocchio.override {
              python3Packages = pkgs.python312Packages;
            };
            py313-pinocchio = self'.packages.py-pinocchio.override {
              python3Packages = pkgs.python313Packages;
            };
          };
        };
    };
}
