cmake_minimum_required(VERSION 3.22.1)
project(test_special_lamp)

set(CMAKE_CXX_STANDARD 20)
#set(CLANG_COMPILE_FLAGS "-Wall")
set(CLANG_COMPILE_FLAGS "-g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_COMPILE_FLAGS}" )

add_executable(test_special_lamp Test.cpp)

target_compile_definitions(test_special_lamp  PRIVATE
        "-DDEBUG"

        PUBLIC
        $<$<CONFIG:Debug>:LAMP_DEBUG>
        $<$<CONFIG:Release>:LAMP_RELEASE>

        $<$<CONFIG:RelWithDebInfo>:LAMP_DEBUG>
        $<$<CONFIG:MinSizeRel>:OE_RELEASE>
)
