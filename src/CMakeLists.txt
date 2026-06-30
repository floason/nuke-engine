set(NUKE_CONFIGURED_HEADER "${PROJECT_BINARY_DIR}/configured/nuke_version.hpp")
configure_file(nuke_version.hpp.in ${NUKE_CONFIGURED_HEADER})
install(FILES ${NUKE_CONFIGURED_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

option(NUKE_BUILD_SHARED "Build Nuke as a shared library instead of static by default" ON)
if(NUKE_BUILD_SHARED)
    set(NUKE_SHARED SHARED)
endif()

add_subdirectory(texture)

add_library(nuke ${NUKE_SHARED} engine.cpp pixelbuffer.cpp sound.cpp)
target_link_libraries(nuke PRIVATE nuke_interface nuke_texture SDL3::SDL3 SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer)
target_include_directories(nuke 
    PUBLIC  "${PROJECT_BINARY_DIR}/configured"
            "${PROJECT_SOURCE_DIR}/include"
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
nuke_compile_export_macros(nuke)

install(TARGETS nuke 
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Create an option for whether to compile the test target.
option(NUKE_BUILD_TEST "Builds the Nuke test target executable." OFF)
if(NUKE_BUILD_TEST)
    add_subdirectory(test)
endif()