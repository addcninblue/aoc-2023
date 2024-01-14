cmake_minimum_required(VERSION 3.15)

# https://nullprogram.com/blog/2023/04/29/
add_definitions(-g3 -Wall -Wextra -Wconversion -Wdouble-promotion -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion -frecord-gcc-switches -march=native)

set(CMAKE_C_STANDARD 17)

project(AdventOfCode2023)
include_directories(${CMAKE_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# file(GLOB_RECURSE C_FILES "*.c")
# MESSAGE("${C_FILES}")
# foreach(C_FILE ${C_FILES})
#    get_filename_component(TARGET_NAME ${C_FILE} NAME_WE)
#    add_executable(${TARGET_NAME} ${C_FILE})
# endforeach()

add_library(unity STATIC shared/unity/unity.c)
target_include_directories(unity PUBLIC shared/unity)

add_executable(1_1 1/1.c)
add_executable(1_2 1/2.c)
# add_executable(2_1 2/1.c)
# add_executable(2_tests 2/tests.c)
# target_link_libraries(2_tests unity)

# add_executable(tagged_ptr_tests tests/tagged_ptr_tests.c)
# target_link_libraries(tagged_ptr_tests unity)

file(GLOB_RECURSE TESTS "tests/*.c")
foreach(TEST ${TESTS})
   get_filename_component(TARGET_NAME ${TEST} NAME_WE)
   add_executable(${TARGET_NAME} ${TEST})
   target_link_libraries(${TARGET_NAME} unity)
   target_compile_definitions(${TARGET_NAME} PRIVATE TEST)
endforeach()

file(GLOB_RECURSE BENCHMARKS "benchmarks/*.c")
foreach(BENCHMARK ${BENCHMARKS})
   get_filename_component(TARGET_NAME ${BENCHMARK} NAME_WE)
   add_executable(${TARGET_NAME} ${BENCHMARK})
   target_link_libraries(${TARGET_NAME} m gcov)
endforeach()
