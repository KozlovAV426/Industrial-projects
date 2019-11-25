cmake_minimum_required(VERSION 3.12)
project(MathParser)

set(CMAKE_CXX_STANDARD 14)

add_executable(MathParser main.cpp Headers/Tree.h Sources/Tree.cpp Headers/Constants.h Headers/TreeManager.h Sources/TreeManager.cpp)