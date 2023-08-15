cmake_minimum_required(VERSION 3.5)

project(Chapter02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01a_func_auto 01a_func_auto.C)
add_executable(01_func_template 01_func_template.C)

add_executable(02_class_template 02_class_template.C)

add_executable(03_variable_template 03_variable_template.C)

add_executable(04_nontype 04_nontype.C)

add_executable(05a_consteval 05a_consteval.C)
add_executable(05b_consteval 05b_consteval.C)
add_executable(05c_consteval 05c_consteval.C)
add_executable(05d_consteval 05d_consteval.C)

add_executable(06a_template_template 06a_template_template.C)
add_executable(06b_template_template 06b_template_template.C)
add_executable(06c_template_template 06c_template_template.C)

add_executable(07_whatami.C 07_whatami.C)

add_executable(08a_variadic_function 08a_variadic_function.C)
add_executable(08b_variadic_function 08b_variadic_function.C)

add_executable(09_variadic_class 09_variadic_class.C)

add_executable(10_variadic_auto 10_variadic_auto.C)

add_executable(11_lambda 11_lambda.C)

add_executable(12a_lambda_overload1 12a_lambda_overload14.C)
add_executable(12b_lambda_overload17 12b_lambda_overload17.C)
add_executable(12c_lambda_overload17 12c_lambda_overload17.C)

add_executable(13a_constraints 13a_constraints.C)
add_executable(13b_constraints 13b_constraints.C)
add_executable(13c_constraints 13c_constraints.C)

add_executable(14a_concept 14a_concept.C)
add_executable(14b_concept 14b_concept.C)

add_executable(15_concept 15_concept.C)
