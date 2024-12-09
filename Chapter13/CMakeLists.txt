cmake_minimum_required(VERSION 3.5)

project(Chapter13 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_construction_hierarchy 01_construction_hierarchy.C)

add_executable(02_construction_hierarchy 02_construction_hierarchy.C)

add_executable(03_construction_hierarchy 03_construction_hierarchy.C)

add_executable(04_virtual_type 04_virtual_type.C)

add_executable(05_factory 05_factory.C)

add_executable(06_factory_unique 06_factory_unique.C)

add_executable(07_factory_method 07_factory_method.C)

add_executable(08_non_factory_ctor 08_non_factory_ctor.C)

add_executable(09_factory_args 09_factory_args.C)

add_executable(10_factory_args 10_factory_args.C)

add_executable(11_spec_args 11_spec_args.C)

add_executable(12_type_registry 12_type_registry.C)

add_executable(13_type_registry 13_type_registry.C)

add_executable(14_type_registry_args 14_type_registry_args.C)

add_executable(15_unit_factory 15_unit_factory.C)

add_executable(16_poly_copy 16_poly_copy.C)

add_executable(17_poly_copy_unique 17_poly_copy_unique.C)

add_executable(18_poly_copy_unique_crtp 18_poly_copy_unique_crtp.C)

add_executable(19_poly_copy_unique_crtp 19_poly_copy_unique_crtp.C)

add_executable(20_poly_copy_crtp 20_poly_copy_crtp.C)

add_executable(21_factory_builder 21_factory_builder.C)
