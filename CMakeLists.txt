cmake_minimum_required(VERSION 3.30)
project(special_lamp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(special_lamp STATIC lampString.cpp)

add_subdirectory(Test)

target_link_libraries(test_special_lamp PRIVATE special_lamp)