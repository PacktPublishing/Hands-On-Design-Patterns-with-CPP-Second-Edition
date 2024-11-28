cmake_minimum_required(VERSION 3.5)

project(Chapter05 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(GTest QUIET)
if (NOT GTest_FOUND)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
  )

  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(googletest)
endif()

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

add_executable(01_benchmark 01_benchmark.C)
target_link_libraries(01_benchmark benchmark::benchmark)

add_executable(02_test 02_test.C)
target_link_libraries(02_test gtest_main)

add_executable(03_object_count 03_object_count.C)
target_link_libraries(03_object_count gtest_main)

add_executable(04_memory 04_memory.C)
target_link_libraries(04_memory gtest_main)

add_executable(05_raii 05_raii.C)
target_link_libraries(05_raii gtest_main)

add_executable(06_scoped_ptr 06_scoped_ptr.C)
target_link_libraries(06_scoped_ptr gtest_main)

add_executable(07_scoped_ptr_bm 07_scoped_ptr_bm.C)
target_link_libraries(07_scoped_ptr_bm benchmark::benchmark)

add_executable(08_mutex_guard 08_mutex_guard.C)
target_link_libraries(08_mutex_guard gtest_main)

add_executable(09_co_resouce 09_co_resouce.C)
add_executable(09a_co_resouce 09a_co_resouce.C)

add_executable(10_co_resource_lambda.C 10_co_resource_lambda.C)
