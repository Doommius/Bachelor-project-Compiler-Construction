cmake_minimum_required(VERSION 3.6)   # CMake version check
project(Compiler)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard



include_directories(include)

add_executable(tests
        include/memory.h
        include/struct.h
        include/symbol.h
        src/tests.c
        src/memory.c
        src/symbol.c
)



add_executable(Compiler
        include/memory.h
        include/struct.h
        include/symbol.h
        src/main.c
        src/memory.c
        src/symbol.c
)
