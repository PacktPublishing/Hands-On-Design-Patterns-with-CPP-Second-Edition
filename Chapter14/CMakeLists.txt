cmake_minimum_required(VERSION 3.5)

project(Chapter14 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_template_method 01_template_method.C)

add_executable(02_polymorphism 02_polymorphism.C)

add_executable(03_template_method 03_template_method.C)

add_executable(04_pre_post_conditions 04_pre_post_conditions.C)

add_executable(05_pre_post_conditions_error 05_pre_post_conditions_error.C)

add_executable(06_pre_post_actions 06_pre_post_actions.C)

add_executable(07_private_virtual 07_private_virtual.C)

add_executable(08_nvi_template_method 08_nvi_template_method.C)

add_executable(09_nvi_re_post_actions 09_nvi_re_post_actions.C)

add_executable(10_nvi_protected 10_nvi_protected.C)

add_executable(11_template_vs_strategy 11_template_vs_strategy.C)

add_executable(12_virtual_destructors 12_virtual_destructors.C)

add_executable(13_fragile_base 13_fragile_base.C)

add_executable(14_fragile_base_bug 14_fragile_base_bug.C)

add_executable(15_template_customization 15_template_customization.C)
