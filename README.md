# Nuke
This is the engine repository for the Nuke 2D game engine. It is written in C++17 and uses [SDL](https://github.com/libsdl-org/SDL) as its multimedia library.

### Using the engine
A future SDK will be created for this engine, which will be designed to be forked from. Users may alternatively seek to write their own SDK for Nuke. This engine propagates specific headers, defined in `include/sdk/`, featuring interfaces which SDK classes must inherit from to establish communications between the SDK and the engine.

### Configuration
When configuring Nuke with CMake before building, the following options are available:
- `NUKE_USE_VENDORED_SDL3` (default: OFF) - add SDL3 as a subdirectory from submodules/SDL instead of using FindSDL.
- - If this option is disabled, by default FindSDL and FindSDL_mixer will search for your devel libraries in external/SDL3/cmake and external/SDL3_image/cmake respectively, alongside any system install paths. This can be changed by modifying `SDL3_DIR` and `SDL3_image_DIR` respectively.
- `NUKE_BUILD_TEST` (default: OFF) - builds the test executable for directly interfacing with the Nuke engine library.
- `NUKE_BUILD_SHARED` (default: ON) - this dictates whether Nuke specifically (rather than all targets via `BUILD_SHARED_LIBS`) is built as a static or shared library. Note that when compiled as a static library, the whole archive should be linked. See [CMake's generator expression for linking an entire static library](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:LINK_LIBRARY) via the `WHOLE_ARCHIVE` linker feature.
- `NUKE_LINK_SDL3_STATIC` (default: OFF) - due to the complexities involved with building SDL3 and SDL3_image statically, it is recommended that this option is toggled instead for linking SDL3/SDL3_image as static libraries.

### Building
This repository uses CMake 3.24 and up for building. Both single- and multi-config generators are supported.

### Dependencies
- [SDL3](https://github.com/libsdl-org/SDL)
- [SDL3_image](https://github.com/libsdl-org/SDL_image)
- [SDL3_mixer](https://github.com/libsdl-org/SDL_mixer)

### Credits
Some of the design philosophy is inspired by [Valve's Source game engine and its respective SDK](https://github.com/valvesoftware/source-sdk-2013).