# This outputs a test target executable for direct engine debugging
# and is not compiled by default!

add_executable(nuke_test main.cpp game_interface.cpp testentity.cpp)
target_link_libraries(nuke_test PRIVATE 
    nuke_interface

    # If compiled statically, all object files must be linked, otherwise
    # not all textures will be compiled!
    $<IF:$<STREQUAL:$<TARGET_PROPERTY:nuke,TYPE>,STATIC_LIBRARY>,
        $<LINK_LIBRARY:WHOLE_ARCHIVE,nuke>,
        nuke>
)
target_include_directories(nuke_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Ensure the SDL3 shared library is present in the build directory of
# the Nuke target for debugging.
get_target_property(SDL3_TYPE SDL3::SDL3 TYPE)
if (SDL3_TYPE STREQUAL "SHARED_LIBRARY")
    add_custom_command(TARGET nuke_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:nuke_test>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_image::SDL3_image>
            $<TARGET_FILE_DIR:nuke_test>
        COMMENT "Copying SDL3 shared libraries to build directory."
    )
endif()

install(TARGETS nuke_test DESTINATION ${CMAKE_INSTALL_BINDIR})