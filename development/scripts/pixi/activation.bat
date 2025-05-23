:: Setup ccache
set CMAKE_CXX_COMPILER_LAUNCHER=ccache

:: Create compile_commands.json for language server
set CMAKE_EXPORT_COMPILE_COMMANDS=1

:: Activate color output with Ninja
set CMAKE_COLOR_DIAGNOSTICS=1

:: Help ccache manage generated files and PCH (https://ccache.dev/manual/latest.html#_precompiled_headers)
set CCACHE_SLOPPINESS=include_file_ctime,include_file_mtime,pch_defines,time_macros

# Set default build value only if not previously set
if not defined PINOCCHIO_BUILD_TYPE (set PINOCCHIO_BUILD_TYPE=Release)
if not defined PINOCCHIO_PYTHON_STUBS (set PINOCCHIO_PYTHON_STUBS=ON)
if not defined PINOCCHIO_COLLISION_SUPPORT (set PINOCCHIO_COLLISION_SUPPORT=OFF)
if not defined PINOCCHIO_ACCELERATE_SUPPORT (set PINOCCHIO_ACCELERATE_SUPPORT=OFF)
if not defined PINOCCHIO_CASADI_SUPPORT (set PINOCCHIO_CASADI_SUPPORT=OFF)
if not defined PINOCCHIO_AUTODIFF_SUPPORT (set PINOCCHIO_AUTODIFF_SUPPORT=OFF)
if not defined PINOCCHIO_EXTRA_SUPPORT (set PINOCCHIO_EXTRA_SUPPORT=OFF)
if not defined PINOCCHIO_OPENMP_SUPPORT (set PINOCCHIO_OPENMP_SUPPORT=OFF)
if not defined PINOCCHIO_CODEGEN_SUPPORT (set PINOCCHIO_CODEGEN_SUPPORT=OFF)
if not defined PINOCCHIO_SDF_SUPPORT (set PINOCCHIO_SDF_SUPPORT=OFF)
if not defined PINOCCHIO_MPFR_SUPPORT (set PINOCCHIO_MPFR_SUPPORT=OFF)
if not defined PINOCCHIO_BUILD_BENCHMARK (set PINOCCHIO_BUILD_BENCHMARK=OFF)
