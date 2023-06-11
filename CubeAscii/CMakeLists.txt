add_executable(${PROJECT_NAME} main.c)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif (UNIX)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm.lib)
endif ()