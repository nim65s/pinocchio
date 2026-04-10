{
  description = "Fast and flexible implementation of Rigid Body Dynamics algorithms and their analytical derivatives.";

  inputs = {
    gepetto.url = "github:gepetto/nix";
    flake-parts.follows = "gepetto/flake-parts";
    systems.follows = "gepetto/systems";
    nixpkgs.url = "github:NixOS/nixpkgs/acb374227d9a4cd0c3eb8406a4f04eeeb4cf1a50";
    gepetto.inputs.gazebros2nix.inputs.flakoboros.inputs.nix-ros-overlay.inputs.nixpkgs.follows =
      "nixpkgs";
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
              extraPyPackages = [
                "example-robot-data"
                "meshcat"
                "viser"
              ];
              overrideAttrs.pinocchio =
                final:
                (super: {
                  cmakeFlags =
                    super.cmakeFlags
                    ++ lib.optionals final.stdenv.hostPlatform.isDarwin [
                      "-DCMAKE_EXE_LINKER_FLAGS=-Wl,-all_load"
                      "-DCMAKE_SHARED_LINKER_FLAGS=-Wl,-all_load"
                    ];
                  patches = [ ];
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
                });
              pyOverrideAttrs.pinocchio =
                _: p:
                (super: {
                  checkInputs = super.checkInputs ++ [ p.pybind11 ];

                });
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
