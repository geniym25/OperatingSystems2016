cmake_minimum_required(VERSION 3.5)
project(task7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES server_tcp.c)
add_executable(task7 ${SOURCE_FILES})