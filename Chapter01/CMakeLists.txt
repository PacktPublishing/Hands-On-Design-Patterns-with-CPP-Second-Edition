cmake_minimum_required(VERSION 3.5)

project(Chapter01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(benchmark QUIET)
if (NOT benchmark_FOUND)
  FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1
  )

  set(BENCHMARK_ENABLE_TESTING off)    # to suppress benchmark internal tests

  FetchContent_MakeAvailable(googlebenchmark)
endif()

add_executable(01_cast 01_cast.C)

add_executable(02_dynamic_cast 02_dynamic_cast.C)
target_link_libraries(02_dynamic_cast benchmark::benchmark)

add_test(NAME 02_dynamic_cast COMMAND 02_dynamic_cast)
