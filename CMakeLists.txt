cmake_minimum_required(VERSION 2.6)

project(fsm-framework)
set(CMAKE_BUILD_TYPE Debug)

add_executable(
    tests
    tests/main.cpp
    tests/state_machine_test.cpp  
    state_machine.c
    )


target_link_libraries(
    tests
    m
    CppUTest
    CppUTestExt
    )
