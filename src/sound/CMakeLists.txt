add_library(nuke_sound INTERFACE)
target_include_directories(nuke_sound INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(nuke_sound INTERFACE sound.cpp soundapi.cpp)