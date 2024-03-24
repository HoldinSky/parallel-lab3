cmake_minimum_required(VERSION 3.26)
project(lab3)

set(CMAKE_CXX_STANDARD 17)

link_directories(./**)

add_executable(app src/main.cpp
        src/common.h
        src/common.cpp
        src/execute_single.cpp
        src/execute_mutex.cpp
        src/execute_atomic.cpp
)
