cmake_minimum_required(VERSION 3.22)
set(TARGET_NAME ColaCore)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} PUBLIC
    spdlog::spdlog
    ColaPlatForm
                )