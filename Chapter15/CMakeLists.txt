cmake_minimum_required(VERSION 3.5)

project(Chapter15 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_scoped_ptr 01_scoped_ptr.C)

add_executable(02_scoped_ptr_deletion_policy 02_scoped_ptr_deletion_policy.C)

add_executable(03_deletion_policy_concepts 03_deletion_policy_concepts.C)

add_executable(04_deletion_policy_assert 04_deletion_policy_assert.C)

add_executable(05_policy_bug 05_policy_bug.C)

add_executable(06_policy_move 06_policy_move.C)

add_executable(07_template_operator 07_template_operator.C)

add_executable(08_template_template 08_template_template.C)

add_executable(09_template_template_variadic 09_template_template_variadic.C)

add_executable(10_policy_base 10_policy_base.C)

add_executable(11_release_policy.C 11_release_policy.C)

add_executable(12_debug_policy 12_debug_policy.C)

add_executable(13_move_policy 13_move_policy.C)

add_executable(14_shared_ptr 14_shared_ptr.C)

add_executable(15_shared_ptr 15_shared_ptr.C)

add_executable(16_shared_ptr_debug 16_shared_ptr_debug.C)

add_executable(17_adapter_using 17_adapter_using.C)

add_executable(18_adapter_inheritance 18_adapter_inheritance.C)

add_executable(19_release_policy_concepts 19_release_policy_concepts.C)

add_executable(20_release_policy_sfinae 20_release_policy_sfinae.C)

add_executable(21_move_policy_concepts 21_move_policy_concepts.C)

add_executable(22_move_policy_sfinae.C 22_move_policy_sfinae.C)

add_executable(23_conversions_concepts 23_conversions_concepts.C)

add_executable(24_conversions_concepts 24_conversions_concepts.C)

add_executable(25_conversions_sfinae_NOT 25_conversions_sfinae_NOT.C)

add_executable(26_conversions_crtp 26_conversions_crtp.C)

add_executable(27_rebind 27_rebind.C)
