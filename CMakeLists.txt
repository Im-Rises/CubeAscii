cmake_minimum_required(VERSION 3.9)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(CubeAscii)

add_executable(CubeAscii main.c)

if (UNIX)
    target_link_libraries(CubeAscii m)
endif(UNIX)
