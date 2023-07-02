cmake_minimum_required(VERSION 3.5)

project(Chapter11 LANGUAGES CXX)

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

add_executable(01_bad 01_bad.C)

add_executable(02_trycatch 02_trycatch.C)

add_executable(02a_trycatch_cleanup 02a_trycatch_cleanup.C)

add_executable(02b_trycatch_raiicleanup 02b_trycatch_raiicleanup.C)

add_executable(03_raii 03_raii.C)

add_executable(04_scopeguard_basic 04_scopeguard_basic.C)

add_executable(04a_scopeguard_basic_typedef 04a_scopeguard_basic_typedef.C)

add_executable(05_scopeguard_basic_move 05_scopeguard_basic_move.C)

add_executable(05a_scopeguard_basic_move_cpp14 05a_scopeguard_basic_move_cpp14.C)

add_executable(06_scopeguard_obj 06_scopeguard_obj.C)

add_executable(07_scopeguard_lambda 07_scopeguard_lambda.C)

add_executable(07a_scopeguard_lambda_cleanup 07a_scopeguard_lambda_cleanup.C)

add_executable(07b_scopeguard_lambda_cpp17 07b_scopeguard_lambda_cpp17.C)

add_executable(08_scopeguard_on_exit 08_scopeguard_on_exit.C)

add_executable(08a_scopeguard_on_exit_named 08a_scopeguard_on_exit_named.C)

add_executable(09_scopeguard_exception 09_scopeguard_exception.C)

add_executable(09a_scopeguard_shielded 09a_scopeguard_shielded.C)

if (NOT MSVC)
  add_executable(10_scopeguard_on_fail 10_scopeguard_on_fail.C)
endif()

add_executable(10a_scopeguard_on_fail_cpp17 10a_scopeguard_on_fail_cpp17.C)

if (NOT MSVC)
  add_executable(10b_scopeguard_on_fail_gcc 10b_scopeguard_on_fail_gcc.C)
endif()

add_executable(10c_scopeguard_on_fail_cpp17_nomacro 10c_scopeguard_on_fail_cpp17_nomacro.C)

add_executable(11_scopeguard_te_benchmark 11_scopeguard_te_benchmark.C)
target_link_libraries(11_scopeguard_te_benchmark benchmark::benchmark)
