cmake_minimum_required(VERSION 3.5)

project(Chapter03 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_non_ownership 01_non_ownership.C)

add_executable(02_non_ownership 02_non_ownership.C)

add_executable(03_unique_ptr 03_unique_ptr.C)

add_executable(04_factory 04_factory.C)

add_executable(05_move 05_move.C)

add_executable(06_shared 06_shared.C)

add_executable(07_string_builder 07_string_builder.C)

add_executable(08_string_factory 08_string_factory.C)

add_executable(09_string_view 09_string_view.C)

add_executable(10_span 10_span.C)

add_executable(11_ranges 11_ranges.C)
