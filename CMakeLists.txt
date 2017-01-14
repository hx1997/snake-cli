cmake_minimum_required(VERSION 3.6)
project(cli_snake)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c console.c console.h)
add_executable(cli_snake ${SOURCE_FILES})