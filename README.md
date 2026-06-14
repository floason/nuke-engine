# Nuke

This is the engine repository for the Nuke 2D game engine.

### Configuration
When configuring Nuke with CMake before building, the following options are available:
- `NUKE_USE_VENDORED_SDL3` (default: OFF) - add SDL3 as a subdirectory from submodules/SDL instead of using FindSDL.
- - If this option is disabled, by default FindSDL will search for your SDL devel install in external/SDL3/cmake, alongside any system install paths. This can be changed by modifying `SDL3_DIR`.
- `NUKE_BUILD_TEST` (default: OFF) - builds the test executable for directly interfacing with the Nuke engine library.
- `NUKE_BUILD_SHARED` (default: ON) - this dictates whether Nuke specifically (rather than all targets via `BUILD_SHARED_LIBS`) is built as a static or shared library.