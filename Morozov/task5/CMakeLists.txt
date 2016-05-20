cmake_minimum_required(VERSION 3.5)
project(task5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c99")

set(SOURCE_FILES main.c)
add_executable(task5 ${SOURCE_FILES})