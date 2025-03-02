cmake_minimum_required(VERSION 3.22)

add_executable(SandBox main.cpp)

target_link_libraries(SandBox PUBLIC ColaCore ColaPlatForm ColaRender)
