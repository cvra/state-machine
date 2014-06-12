cmake_minimum_required(VERSION 2.6)

project(fsm-framework)

# Check for CppUTest and bail out if they don't have it
if(DEFINED ENV{CPPUTEST_HOME})
    message("Using CppUTest found in $ENV{CPPUTEST_HOME}")
else()
    message(FATAL_ERROR "CPPUTEST_HOME is not set; You must tell CMake where to find CppUTest")
endif()

set(CMAKE_BUILD_TYPE Debug)


include_directories($ENV{CPPUTEST_HOME}/include)
link_directories($ENV{CPPUTEST_HOME}/lib)

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
