cmake_minimum_required(VERSION 3.28)
project(My_C_Virtual_Machine)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        My_C_Virtual_Machine main.cpp
        Virtual_Machine.cpp
        Virtual_Machine.h
        train.c
        test.c
)
