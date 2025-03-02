cmake_minimum_required(VERSION 3.22)
project(ColaEngine VERSION 0.1.0 LANGUAGES C CXX)

# 获取OS信息
if (CMAKE_HOST_WIN32)
    message("Platform: Windows")
    add_definitions(-DENGINE_PLATFORM_WIN32)
elseif(CMAKE_HOST_APPLE)
    message("Platform: Macos")
    add_definitions(-DENGINE_PLATFORM_MACOS)
elseif(CMAKE_HOST_UNIX)
    message("Platform: Linux")
    add_definitions(-DENGINE_PLATFORM_LINUX)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_definitions(-DCola_DEFINE_RES_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/Resource/\")
set(Cola_DEFINE_RES_ROOT_DIR "${CMAKE_SOURCE_DIR}/Resource/")

# include_directories(./include/)
# aux_source_directory(./src/ SOURCES)
# link_directories(./lib/)

# set(Vulkan_LIBRARIES vulkan-1.lib)

# add_executable(ColaEngine ${SOURCES})

# target_include_directories(ColaEngine PRIVATE
#     ./include/glm
#     ./include/vulkan
#     ./include/vulkan/utility
# )

# target_link_libraries(ColaEngine
#     ${Vulkan_LIBRARIES}
#  )

include_directories(Platform)
include_directories(Core)
include_directories(Function)

add_subdirectory(3rdparty)
add_subdirectory(Platform)
add_subdirectory(Core)
add_subdirectory(Function)
add_subdirectory(Editor)
add_subdirectory(Sample)



