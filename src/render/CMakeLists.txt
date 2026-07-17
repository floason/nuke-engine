add_library(nuke_render INTERFACE)
target_include_directories(nuke_render INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(nuke_render INTERFACE pixelbuffer.cpp renderer.cpp text_interface.cpp)