cmake_minimum_required(VERSION 3.5)
project(task4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu99")

set(SOURCE_FILES main.c)
add_executable(task4 ${SOURCE_FILES})