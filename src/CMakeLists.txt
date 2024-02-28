add_subdirectory(libs)

if (NOT LIBRARY_ONLY)
    add_executable(lab1_task1 lab1/task1/main.cpp)
    add_executable(lab1_task2 lab1/task2/main.cpp)
    add_executable(lab2 lab2/main.cpp)
    # target_link_libraries(lab1 alg)
endif()