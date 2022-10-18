cmake_minimum_required(VERSION 3.5)

project(Chapter01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.0)
FetchContent_MakeAvailable(
    googletest
    googlebenchmark)

add_executable(01_cast 01_cast.C)

add_executable(02_dynamic_cast 02_dynamic_cast.C)
target_link_libraries(02_dynamic_cast benchmark::benchmark)
add_test(NAME 02_dynamic_cast COMMAND 02_dynamic_cast)
