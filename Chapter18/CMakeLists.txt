cmake_minimum_required(VERSION 3.5)

project(Chapter18 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_mutex 01_mutex.C)

add_executable(02_race 02_race.C)

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_executable(03_mutex_cpp20 03_mutex_cpp20.C)
endif()

add_executable(04_mutex_guarded 04_mutex_guarded.C)

if (NOT MSVC)
  add_executable(05_shared_count 05_shared_count.C)

  add_executable(06_local_count 06_local_count.C)

  add_executable(07_per_thread 07_per_thread.C)

  add_executable(08_per_thread 08_per_thread.C)
endif()

add_executable(09_per_thread 09_per_thread.C)

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_executable(10_copy 10_copy.C)
endif()

add_executable(11_wait_notify.C 11_wait_notify.C)

add_executable(12_wait_poll 12_wait_poll.C)

add_executable(13_wait_notify_atomic 13_wait_notify_atomic.C)

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_executable(14_latch 14_latch.C)
endif()

if (NOT MSVC)
  add_executable(15_atomic_count 15_atomic_count.C)
endif()

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_executable(16_atomic_index 16_atomic_index.C)
endif()

add_executable(17_publish_ptr 17_publish_ptr.C)

add_executable(18_publish_index 18_publish_index.C)

add_executable(19_publish_mt 19_publish_mt.C)

add_executable(20_locked_queue 20_locked_queue.C)

add_executable(21_lock_free_queue 21_lock_free_queue.C)

add_executable(22_active_object.C 22_active_object.C)

add_executable(23_active_object_poly 23_active_object_poly.C)

add_executable(24_active_object_te 24_active_object_te.C)

if (NOT MSVC)
  add_executable(25_reactor 25_reactor.C)
endif()

add_executable(26_proactor 26_proactor.C)

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_executable(27_monitor 27_monitor.C)
endif()

add_executable(28_generator_coro 28_generator_coro.C)

add_executable(29_generator_coro.C 29_generator_coro.C)
