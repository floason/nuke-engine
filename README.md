# Nuke

This is the engine repository for the Nuke 2D game engine.

### Configuration
When configuring Nuke with CMake before building, the following options are available:
- `NUKE_USE_VENDORED_SDL3` (default: OFF) - add SDL3 as a subdirectory from submodules/SDL instead of using FindSDL.
- - If this option is disabled, by default FindSDL will search for your SDL devel install in external/SDL3/cmake, alongside any system install paths. This can be changed by modifying `SDL3_DIR`.
- `NUKE_BUILD_TEST` (default: OFF) - builds the test executable for directly interfacing with the Nuke engine library.
- `BUILD_SHARED_LIBS` (default: OFF) - a built-in CMake option, this dictates whether Nuke is built as a static or shared library.