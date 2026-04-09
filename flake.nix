{
  description = "Fast and flexible implementation of Rigid Body Dynamics algorithms and their analytical derivatives.";

  inputs = {
    gepetto.url = "github:gepetto/nix";
    flake-parts.follows = "gepetto/flake-parts";
    systems.follows = "gepetto/systems";
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } (
      { lib, ... }:
      {
        systems = import inputs.systems;
        imports = [
          inputs.gepetto.flakeModule
          {
            flakoboros = {
              extraDevPyPackages = [ "pinocchio" ];
              extraPyPackages = [
                "example-robot-data"
                "meshcat"
                "viser"
              ];
              overrideAttrs.pinocchio = _: {
                src = lib.fileset.toSource {
                  root = ./.;
                  fileset = lib.fileset.unions [
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
              };
              extends = {
                full = _final: prev: {
                  pythonPackagesExtensions = prev.pythonPackagesExtensions ++ [
                    (_python-final: python-prev: {
                      pinocchio = python-prev.pinocchio.override { buildStandalone = false; };
                    })
                  ];
                };
              };
            };
          }
        ];
      }
    );
}
