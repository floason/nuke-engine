# This outputs a test target executable for direct engine debugging
# and is not compiled by default!

add_executable(nuke_test main.cpp)
target_link_libraries(nuke_test PRIVATE nuke)