cmake_minimum_required(VERSION 3.22)

add_executable(VulkanTriangle main.cpp)

target_link_libraries(VulkanTriangle PUBLIC ColaCore ColaPlatForm ColaRender)
