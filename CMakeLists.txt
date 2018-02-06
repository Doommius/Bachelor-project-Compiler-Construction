cmake_minimum_required(VERSION 3.9)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(Compiler
        include/memory.h
        include/struct.h
        include/symbol.h
        lib/README.md
        src/modules/README.md
        src/main.c
        src/memory.c
        src/symbol.c
        LICENSE
        README.md)
