cmake_minimum_required(VERSION 3.31)
project(readchar)

set(CMAKE_CXX_STANDARD 17)

add_library(readchar STATIC library.cpp
        win_readchar.cpp
        win_readchar.h
        posix_readchar.cpp
        posix_readchar.h
        utf8.h
)
