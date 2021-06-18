# Link-time plug-ins example program
# Build configuration
# by Wolfram Rösler 2018-06-25

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
project(demo)

add_executable(demo1
    demo-main.cpp
    demo-cat.cpp
    demo-dog.cpp
    demo-bird.cpp
)

add_executable(demo2
    demo-main.cpp
    demo-cat.cpp
)

add_executable(demo3
    demo-main.cpp
    demo-cat.cpp
    demo-bird.cpp
)
