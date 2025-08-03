cmake_minimum_required(VERSION 3.14)
project(CppTestFramework)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(test/sample_test)
add_subdirectory(test/basic_test)
add_subdirectory(test/new_test)
add_subdirectory(test/stack_test)
add_subdirectory(test/queue_test)
add_subdirectory(test/bst_test)
add_subdirectory(test/avl_test)
