cmake_minimum_required(VERSION 3.17)
project(UDPLib C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_library(UDPLib include/UDP.h src/UDP.c)
add_subdirectory(tests)