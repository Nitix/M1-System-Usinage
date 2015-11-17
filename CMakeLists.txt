cmake_minimum_required(VERSION 3.3)
project(M1-System-Usinage)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c)
add_executable(M1-System-Usinage ${SOURCE_FILES} main.c)