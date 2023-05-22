add_executable(CubeAscii main.c)

if (UNIX)
    target_link_libraries(CubeAscii m)
endif (UNIX)
