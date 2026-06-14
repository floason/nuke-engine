set(NUKE_CONFIGURED_HEADER "${PROJECT_BINARY_DIR}/configured/nuke_version.hpp")
configure_file(nuke_version.hpp.in ${NUKE_CONFIGURED_HEADER})
install(FILES ${NUKE_CONFIGURED_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_library(nuke main.cpp)
target_link_libraries(nuke PRIVATE nuke_interface SDL3::SDL3)
target_include_directories(nuke 
    PUBLIC  "${PROJECT_BINARY_DIR}/configured"
            "${PROJECT_SOURCE_DIR}/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/src"
)
nuke_compile_export_macros(nuke)

# Ensure the SDL3 shared library is present in the build directory of
# the Nuke target for debugging.
get_target_property(SDL3_TYPE SDL3::SDL3 TYPE)
if (SDL3_TYPE STREQUAL "SHARED_LIBRARY")
    add_custom_command(TARGET nuke POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Copying SDL3 shared library to build directory."
    )
endif()

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