cmake_minimum_required(VERSION 3.14)
project(minifsm)

set(CMAKE_CXX_STANDARD 17)

add_executable(minifsm example.cpp)

target_include_directories(minifsm PRIVATE ${CMAKE_SOURCE_DIR}/include)