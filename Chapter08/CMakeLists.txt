cmake_minimum_required(VERSION 3.5)

project(Chapter08 LANGUAGES CXX)

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

add_executable(01_function_call 01_function_call.C)
target_link_libraries(01_function_call benchmark::benchmark)

add_executable(01a_nested_types 01a_nested_types.C)

add_executable(02_bad_crtp 02_bad_crtp.C)

add_executable(03_good_crtp 03_good_crtp.C)

add_executable(04_operator_plus 04_operator_plus.C)

add_executable(05_operator_plus 05_operator_plus.C)

add_executable(06_compare 06_compare.C)
add_executable(06a_compare_cpp20 06a_compare_cpp20.C)

add_executable(07_operator_plus_concepts 07_operator_plus_concepts.C)

add_executable(08_registry 08_registry.C)

add_executable(09_visitor 09_visitor.C)

add_executable(10_policies 10_policies.C)

add_executable(11_template_policies 11_template_policies.C)
