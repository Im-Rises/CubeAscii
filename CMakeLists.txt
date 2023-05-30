cmake_minimum_required(VERSION 3.9)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(CubeAscii)

cmake_policy(SET CMP0079 NEW)

add_subdirectory(${PROJECT_NAME})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm.lib)
endif ()