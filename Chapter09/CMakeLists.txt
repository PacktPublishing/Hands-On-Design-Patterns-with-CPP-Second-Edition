cmake_minimum_required(VERSION 3.5)

project(Chapter09 LANGUAGES CXX)

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

add_executable(01_city 01_city.C)

add_executable(02_cout_chaining 02_cout_chaining.C)

add_executable(03_city_named_args 03_city_named_args.C)

add_executable(04_named_args 04_named_args.C)
target_link_libraries(04_named_args benchmark::benchmark)

add_executable(05_hier_chain 05_hier_chain.C)

add_executable(06a_poly_chain 06a_poly_chain.C)
add_executable(06b_poly_chain 06b_poly_chain.C)

add_executable(07_crtp_chain 07_crtp_chain.C)

add_executable(08_builder_vector 08_builder_vector.C)

add_executable(09_builder_employee 09_builder_employee.C)

add_executable(10_builder_html 10_builder_html.C)

add_executable(11_builder_html 11_builder_html.C)

add_executable(12_builder_html_cpp20 12_builder_html_cpp20.C)
