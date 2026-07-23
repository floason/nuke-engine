add_library(nuke_netcode INTERFACE)
target_include_directories(nuke_netcode INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(nuke_netcode INTERFACE netapi.cpp server.cpp client.cpp)