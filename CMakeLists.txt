cmake_minimum_required(VERSION 3.26)
project(OS_wokerbench)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/shared_head)


add_executable(OS_wokerbench src/pstree/pstree.cpp
        src/main.cpp
        src/libco/co.cpp)

