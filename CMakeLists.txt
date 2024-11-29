cmake_minimum_required(VERSION 3.30)
project(special_lamp)

set(CMAKE_CXX_STANDARD 20)

add_library(special_lamp STATIC lampString.cpp
        lampString.h)
