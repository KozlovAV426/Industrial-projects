cmake_minimum_required(VERSION 3.12)
project(test_task)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(tests/googletest)

include_directories(tests/googletest/include)

include_directories(tests/googlemock/include)

add_executable(test_task main.cpp Header.h Defenitions.cpp tests/testing.cpp)

target_link_libraries(test_task gtest gtest_main)