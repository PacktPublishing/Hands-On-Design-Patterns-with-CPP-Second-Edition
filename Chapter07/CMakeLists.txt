cmake_minimum_required(VERSION 3.5)

project(Chapter07 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_overload 01_overload.C)

add_executable(02_overload 02_overload.C)

add_executable(03_overload 03_overload.C)

add_executable(04_template_overload 04_template_overload.C)

add_executable(05_template_overload 05_template_overload.C)

add_executable(06_vararg_overload 06_vararg_overload.C)

add_executable(07_deduction 07_deduction.C)

add_executable(08_default_args 08_default_args.C)

add_executable(09_substitution 09_substitution.C)

add_executable(10_sfinae 10_sfinae.C)

add_executable(11_sfinae 11_sfinae.C)

add_executable(12_sfinae 12_sfinae.C)

add_executable(13_sfinae 13_sfinae.C)

add_executable(14_error 14_error.C)

add_executable(15a_class_template 15a_class_template.C)

add_executable(15b_class_template 15b_class_template.C)

add_executable(16_sfinae_natural 16_sfinae_natural.C)

add_executable(17_substitution_error 17_substitution_error.C)

add_executable(18_sfinae_artificial 18_sfinae_artificial.C)

add_executable(19_constraints 19_constraints.C)

add_executable(20_require_expression 20_require_expression.C)

add_executable(20a_require_expression 20a_require_expression.C)

add_executable(21_concepts 21_concepts.C)

add_executable(22_trait_has_value_type 22_trait_has_value_type.C)

add_executable(22a_trait_has_value_type 22a_trait_has_value_type.C)

add_executable(23_trait_is_class 23_trait_is_class.C)

add_executable(24_pseudo_concepts 24_pseudo_concepts.C)

add_executable(24a_pseudo_concepts 24a_pseudo_concepts.C)

add_executable(25_pseudo_concepts 25_pseudo_concepts.C)
