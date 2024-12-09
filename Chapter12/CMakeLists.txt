cmake_minimum_required(VERSION 3.5)

project(Chapter12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_no_access 01_no_access.C)

add_executable(02_friend 02_friend.C)

add_executable(03_member_plus 03_member_plus.C)

add_executable(04_friend_plus 04_friend_plus.C)

add_executable(05_friend_plus 05_friend_plus.C)

add_executable(06_stream_friend 06_stream_friend.C)

add_executable(07_stream_friend 07_stream_friend.C)

add_executable(08_friend_forward 08_friend_forward.C)

add_executable(09_friend_forward 09_friend_forward.C)

add_executable(10_friend_scope 10_friend_scope.C)

add_executable(11_friend_scope 11_friend_scope.C)

add_executable(12_friend_scope 12_friend_scope.C)

add_executable(13_operator_new 13_operator_new.C)

add_executable(14_operator_new 14_operator_new.C)

add_executable(15_friend_template 15_friend_template.C)

add_executable(16_friend_template 16_friend_template.C)

add_executable(17_friend_factory 17_friend_factory.C)

add_executable(18_friend_factory_ns 18_friend_factory_ns.C)

add_executable(19_crtp_equal_factory 19_crtp_equal_factory.C)

add_executable(20_crtp_plus_factory 20_crtp_plus_factory.C)
