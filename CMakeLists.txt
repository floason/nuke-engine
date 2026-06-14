cmake_minimum_required(VERSION 3.19)
include(GNUInstallDirs)

project(nuke VERSION 0.0.0 LANGUAGES C CXX)

add_library(nuke_interface INTERFACE)
target_compile_features(nuke_interface INTERFACE cxx_std_11)

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

# This engine uses SDL3 so as to power the Rust 2D engine as a whole.
option(NUKE_USE_VENDORED_SDL3 "Compile SDL3 submodule instead of using SDL3 package" OFF)
if(NUKE_USE_VENDORED_SDL3)
    if(NOT SDL3_DIR)
        set(SDL3_DIR external/SDL3/cmake)
    endif()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
else()
    add_subdirectory(submodules/SDL EXCLUDE_FROM_ALL)
endif()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
add_subdirectory(src)