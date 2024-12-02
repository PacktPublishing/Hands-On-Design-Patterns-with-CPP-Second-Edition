cmake_minimum_required(VERSION 3.5)

project(Chapter06 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
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

add_executable(01_unique_ptr 01_unique_ptr.C)

add_executable(02_unique_ptr 02_unique_ptr.C)

add_executable(03_shared_ptr 03_shared_ptr.C)

add_executable(04_std_function 04_std_function.C)

add_executable(05_dependencies 05_dependencies.C)

add_executable(06_c_sort 06_c_sort.C)

add_executable(07_smartptr 07_smartptr.C 07_smartptr_ext.C 07_smartptr.h)
target_link_libraries(07_smartptr benchmark::benchmark)

add_executable(08_any 08_any.C)

add_executable(09_function 09_function.C)
target_link_libraries(09_function benchmark::benchmark)
