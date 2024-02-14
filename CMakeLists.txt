cmake_minimum_required(VERSION 3.16)
project(operations_research CXX)

message("LIBRARY_ONLY=${LIBRARY_ONLY}")

add_subdirectory(src)

if (NOT LIBRARY_ONLY)
    enable_testing()
    add_subdirectory(test)
endif()

# install(TARGETS alg DESTINATION .)