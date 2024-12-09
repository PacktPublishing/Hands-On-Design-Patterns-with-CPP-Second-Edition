cmake_minimum_required(VERSION 3.5)

project(Chapter10 LANGUAGES CXX)

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

add_executable(01_malloc_free 01_malloc_free.C)
target_link_libraries(01_malloc_free benchmark::benchmark)

add_executable(02_malloc_free 02_malloc_free.C)
target_link_libraries(02_malloc_free benchmark::benchmark)

add_executable(03_malloc_free_mt 03_malloc_free_mt.C)
target_link_libraries(03_malloc_free_mt benchmark::benchmark)

add_executable(04_small_string 04_small_string.C)
target_link_libraries(04_small_string benchmark::benchmark)

add_executable(05_small_string 05_small_string.C)
target_link_libraries(05_small_string benchmark::benchmark)

add_executable(06_small_vector 06_small_vector.C)
target_link_libraries(06_small_vector benchmark::benchmark)

add_executable(07_small_vector 07_small_vector.C)
target_link_libraries(07_small_vector benchmark::benchmark)

add_executable(08_small_queue 08_small_queue.C)
target_link_libraries(08_small_queue benchmark::benchmark)

add_executable(09_smartptr 09_smartptr.C)
target_link_libraries(09_smartptr benchmark::benchmark)

add_executable(10_local_buffer 10_local_buffer.C)
target_link_libraries(10_local_buffer benchmark::benchmark)

add_executable(11_local_buffer_vtable 11_local_buffer_vtable.C)
target_link_libraries(11_local_buffer_vtable benchmark::benchmark)
