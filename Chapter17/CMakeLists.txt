cmake_minimum_required(VERSION 3.5)

project(Chapter17 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_visitor 01_visitor.C)

add_executable(02_visitor 02_visitor.C)

add_executable(03_visitor 03_visitor.C)

add_executable(04_visitor_args 04_visitor_args.C)

add_executable(05_visitor_retval 05_visitor_retval.C)

add_executable(06_container_visitor 06_container_visitor.C)

add_executable(07_composite_visitor 07_composite_visitor.C)

add_executable(08_serialization_visitor 08_serialization_visitor.C)

add_executable(09_serialization_visitor 09_serialization_visitor.C)

add_executable(10_acyclic_visitor 10_acyclic_visitor.C)

add_executable(11_visitor_template.C 11_visitor_template.C)

add_executable(12_visitor_template 12_visitor_template.C)

add_executable(13_visitor_lambda 13_visitor_lambda.C)

add_executable(14_visitor_lambda_tuple 14_visitor_lambda_tuple.C)

add_executable(15_acyclic_visitor 15_acyclic_visitor.C)

add_executable(16_acyclic_visitor 16_acyclic_visitor.C)

add_executable(17_acyclic_visitor 17_acyclic_visitor.C)

add_executable(18_static_visitor 18_static_visitor.C)

add_executable(19_serialize_visitor 19_serialize_visitor.C)

add_executable(20_serialize_visitor 20_serialize_visitor.C)

add_executable(21_visitor_variant 21_visitor_variant.C)

add_executable(22_visitor_variant_lambda.C 22_visitor_variant_lambda.C)

add_executable(23_visitor_variants 23_visitor_variants.C)
