cmake_minimum_required(VERSION 3.5)

project(Chapter16 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_decorator 01_decorator.C)

add_executable(02_decorator 02_decorator.C)

add_executable(03_decorator 03_decorator.C)

add_executable(04_decorator 04_decorator.C)

add_executable(05_debug_decorator 05_debug_decorator.C)

add_executable(06_debug_decorator 06_debug_decorator.C)

add_executable(07_universal_decorator 07_universal_decorator.C)

add_executable(08_universal_decorator_cpp17 08_universal_decorator_cpp17.C)

add_executable(08a_universal_decorator 08a_universal_decorator.C)

if (NOT MSVC)
  add_executable(09_universal_decorator 09_universal_decorator.C)
endif()

add_executable(10_universal_decorator.C 10_universal_decorator.C)

add_executable(11_locking_queue 11_locking_queue.C)

add_executable(12_locking_queue 12_locking_queue.C)

add_executable(13_adapter_curry 13_adapter_curry.C)

add_executable(14_adapter_curry 14_adapter_curry.C)

add_executable(15_adapter_lambda 15_adapter_lambda.C)

add_executable(15a_adapter_lambda 15a_adapter_lambda.C)

add_executable(16_adapter_lambda 16_adapter_lambda.C)

add_executable(17_adapter_bind 17_adapter_bind.C)

add_executable(18_time_adapter 18_time_adapter.C)

add_executable(19_time_adapter 19_time_adapter.C)

add_executable(20_template_template_adapter.C 20_template_template_adapter.C)

add_executable(21_template_template_adapter 21_template_template_adapter.C)

add_executable(22_template_template_adapter 22_template_template_adapter.C)

add_executable(23_template_template_adapter 23_template_template_adapter.C)

add_executable(24_value_decorators 24_value_decorators.C)

add_executable(25_value_decorators_crtp 25_value_decorators_crtp.C)

if (NOT MSVC)
  add_executable(26_value_policy 26_value_policy.C)
endif()
