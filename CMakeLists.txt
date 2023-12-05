cmake_minimum_required(VERSION 3.15)

# https://nullprogram.com/blog/2023/04/29/
add_definitions(-g3 -Wall -Wextra -Wconversion -Wdouble-promotion -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion)

project(AdventOfCode2023)
include_directories(${CMAKE_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# file(GLOB_RECURSE C_FILES "*.c")
# MESSAGE("${C_FILES}")
# foreach(C_FILE ${C_FILES})
#    get_filename_component(TARGET_NAME ${C_FILE} NAME_WE)
#    add_executable(${TARGET_NAME} ${C_FILE})
# endforeach()

add_executable(1_1 1/1.c)
add_executable(1_2 1/2.c)
