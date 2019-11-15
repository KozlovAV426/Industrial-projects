cmake_minimum_required(VERSION 3.12)
project(Akinator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Akinator main.cpp Tree.h Config.h Tree.cpp Akinator.h Akinator.cpp AkinatorManager.h)