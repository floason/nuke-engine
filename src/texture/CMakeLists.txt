add_library(nuke_texture INTERFACE)
target_include_directories(nuke_texture INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(nuke_texture INTERFACE nuke_fonts)
target_sources(nuke_texture INTERFACE texture_base.cpp texture_rect.cpp texture_sdl.cpp 
    texture_image.cpp texture_stream.cpp texture_test.cpp texture_text.cpp)

add_subdirectory(fonts)