cmake_minimum_required(VERSION 3.12)
project(UnkillableStack)

set(CMAKE_CXX_STANDARD 14)

#include_directories("${PROJECT_SOURCE_DIR}")

#EXECUTE_PROCESS(COMMAND python "${PROJECT_SOURCE_DIR}/undef_adder.py"
        #WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(UnkillableStack main.cpp Stack.h Dump.cpp)