cmake_minimum_required(VERSION 3.24)
include(GNUInstallDirs)

project(nuke VERSION 0.3.0 LANGUAGES C CXX)

# This engine uses SDL3 so as to power the Rust 2D engine as a whole.
option(NUKE_USE_VENDORED_SDL3 "Compile SDL3 submodule instead of using SDL3 package" OFF)
option(NUKE_LINK_SDL3_STATIC "Link SDL3/SDL3_image as static libraries" OFF)

if(NOT NUKE_USE_VENDORED_SDL3)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3")
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3/cmake")
    endif()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3_image")
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3_image/cmake")
    endif()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3_mixer")
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3_mixer/cmake")
    endif()

    if(NUKE_LINK_SDL3_STATIC)
        find_package(SDL3 REQUIRED COMPONENTS SDL3-static)
        find_package(SDL3_image REQUIRED COMPONENTS SDL3_image-static)
        find_package(SDL3_mixer REQUIRED COMPONENTS SDL3_mixer-static)
    else()
        find_package(SDL3 REQUIRED COMPONENTS SDL3-shared)
        find_package(SDL3_image REQUIRED COMPONENTS SDL3_image-shared)
        find_package(SDL3_mixer REQUIRED COMPONENTS SDL3_mixer-shared)
    endif()
else()
    set(NUKE_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

    if(NUKE_LINK_SDL3_STATIC)
        set(SDL_SHARED OFF CACHE BOOL "" FORCE)
        set(SDL_STATIC ON CACHE BOOL "" FORCE)
        add_subdirectory(submodules/SDL EXCLUDE_FROM_ALL)

        # For some reason, SDL_image's CMakeLists.txt decides which SDL3 target
        # to link against based on the value of SDLIMAGE_BUILD_SHARED_LIBS...
        # which by default is set to ${BUILD_SHARED_LIBS}. This needs to be
        # forcibly overwritten based on how SDL3 is being linked by Nuke!
        set(BUILD_SHARED_LIBS OFF)
    else()
        set(SDL_SHARED ON CACHE BOOL "" FORCE)
        set(SDL_STATIC OFF CACHE BOOL "" FORCE)

        add_subdirectory(submodules/SDL EXCLUDE_FROM_ALL)

        set(BUILD_SHARED_LIBS ON)
    endif()

    add_subdirectory(submodules/SDL_image EXCLUDE_FROM_ALL)
    add_subdirectory(submodules/SDL_mixer EXCLUDE_FROM_ALL)
    set(BUILD_SHARED_LIBS ${NUKE_BUILD_SHARED_LIBS})
endif()

add_library(nuke_interface INTERFACE)
target_compile_features(nuke_interface INTERFACE cxx_std_17)

# Called on target libraries that export certain definitions. This defines
# the NUKE_EXPORT macro for the supplied library while also making all symbols
# hidden by default. Additionally, the NUKE_SHARED_LIBRARY macro is also
# compiled into the target if the target is a shared library (configurable
# via BUILD_SHARED_LIBS).
function(nuke_compile_export_macros TARGET_NAME)
    target_compile_definitions(${TARGET_NAME} PRIVATE NUKE_EXPORT)
    set_target_properties(${TARGET_NAME} PROPERTIES 
        LANG_VISIBILITY_PRESET HIDDEN
        VISIBILITY_INLINES_HIDDEN YES
    )

    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)
    if(TARGET_TYPE STREQUAL "SHARED_LIBRARY")
        target_compile_definitions(${TARGET_NAME} PRIVATE NUKE_SHARED_LIBRARY)
    endif()
endfunction()

# Ignore MSVC security warnings.
target_compile_definitions(nuke_interface INTERFACE _CRT_SECURE_NO_WARNINGS)

# Binaries should be compiled to build/out/ARCH/CONFIG/PLATFORM/.
# Currently, only common IA-32/AMD64 processor platform names are normalised,
# including specific toolchain variants.
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" NUKE_ARCH)
if(NUKE_ARCH MATCHES "^(x86_64|amd64)(.*)")
    set(NUKE_ARCH "x86_64")
elseif(NUKE_ARCH MATCHES "^(i[3-6]86|x86)(.*)")
    set(NUKE_ARCH "x86")
endif()
set(NUKE_CONFIG "$<IF:$<BOOL:$<CONFIG>>,$<CONFIG>,Base>")
set(NUKE_OUTPUT_DIR "${PROJECT_BINARY_DIR}/out/${NUKE_ARCH}/${NUKE_CONFIG}/$<PLATFORM_ID>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${NUKE_OUTPUT_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${NUKE_OUTPUT_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${NUKE_OUTPUT_DIR}/bin")

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
add_subdirectory(src)