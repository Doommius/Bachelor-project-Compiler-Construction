cmake_minimum_required(VERSION 3.6)   # CMake version check
project(Compiler)                     # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

find_package(BISON)
find_package(FLEX)

FLEX_TARGET(Scanner src/modules/scanner/flex/exp.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)

#Bison Flags -v -r all usefull for debugging
BISON_TARGET(Parser src/modules/parser/bison/exp.y ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c)

ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

include_directories(include)
include_directories(src/in)

set(SOURCE_FILES_compiler

        src/main.c
        src/memory.c
        src/auxiliary.c

        include/kind.h
        include/memory.h
        include/auxiliary.h

        src/modules/code/code.c
        src/modules/code/include/code.h

        src/modules/error/error.c
        src/modules/error/include/error.h

        src/modules/optimizer/optimizer.c
        src/modules/optimizer/include/optimzer.h

        src/modules/parser/tree.c
        src/modules/parser/include/tree.h

        src/modules/pretty/pretty.c
        src/modules/pretty/include/pretty.h

        src/modules/resource/resources.c
        src/modules/resource/include/resources.h

        src/modules/symbol_tree/symbol.c
        src/modules/symbol_tree/include/symbol.h

        src/modules/typechecker/typechecker.c
        src/modules/typechecker/include/typechecker.h

        src/modules/weeder/weeder.c
        src/modules/weeder/include/weeder.h
        )

add_executable(Compiler
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
        ${SOURCE_FILES_compiler}
        )


add_executable(Z_Playground_Mark_1
        litter/playground/Mark_1/main.c
        )