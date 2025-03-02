cmake_minimum_required(VERSION 3.22)

file(GLOB_RECURSE RENDER_HEADER_FILES "*.h")
file(GLOB_RECURSE RENDER_SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${RENDER_HEADER_FILES} ${RENDER_SOURCE_FILES})

add_library(ColaRender ${RENDER_HEADER_FILES} ${RENDER_SOURCE_FILES})

find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
    message("----> Find vulkan success : ${Vulkan_INCLUDE_DIRS}")
endif()

target_link_libraries(ColaRender PUBLIC glfw ColaCore ColaPlatForm)
target_include_directories(ColaRender PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(ColaRender PRIVATE ${Vulkan_LIBRARIES})