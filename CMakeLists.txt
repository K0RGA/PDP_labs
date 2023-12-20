cmake_minimum_required(VERSION 3.24)
project(RPR_lab1)

set(CMAKE_CXX_STANDARD 17)

add_executable(RPR_lab1 main.cpp)

SET_TARGET_PROPERTIES(RPR_lab1 PROPERTIES COMPILE_FLAGS "-fopenmp" LINK_FLAGS "-fopenmp")


