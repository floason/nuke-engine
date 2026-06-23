# This outputs a test target executable for direct engine debugging
# and is not compiled by default!

add_executable(nuke_test main.cpp game_interface.cpp testentity.cpp)
target_link_libraries(nuke_test PRIVATE 
    nuke_interface

    # If compiled statically, all object files must be linked, otherwise
    # not all textures will be compiled!
    "$<LINK_LIBRARY:WHOLE_ARCHIVE,nuke>"
)
target_include_directories(nuke_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS nuke_test DESTINATION ${CMAKE_INSTALL_BINDIR})