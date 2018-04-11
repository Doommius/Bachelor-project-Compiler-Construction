cmake_minimum_required(VERSION 3.6)   # CMake version check
project(compiler)                     # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

find_package(BISON)
find_package(FLEX)

FLEX_TARGET(Scanner src/modules/scanner/flex/exp.l ${CMAKE_CURRENT_BINARY_DIR}/../src/lex.yy.c)

#Bison Flags -v -r all usefull for debugging
BISON_TARGET(Parser src/modules/parser/bison/exp.y ${CMAKE_CURRENT_BINARY_DIR}/../src/y.tab.c)

ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

include_directories(include)
include_directories(src/modules/code)
include_directories(src/modules/code/include)
include_directories(src/modules/error)
include_directories(src/modules/error/include)
include_directories(src/modules/optimizer)
include_directories(src/modules/optimizer/include)
include_directories(src/modules/parser)
include_directories(src/modules/parser/include)
include_directories(src/modules/pretty)
include_directories(src/modules/pretty/include)
include_directories(src/modules/resource)
include_directories(src/modules/resource/include)
include_directories(src/modules/symbol_tree)
include_directories(src/modules/symbol_tree/include)
include_directories(src/modules/typechecker)
include_directories(src/modules/typechecker/include)
include_directories(src/modules/weeder)
include_directories(src/modules/weeder/include)
include_directories(src/modules/print_asm)
include_directories(src/modules/print_asm/include)

set(SOURCE_FILES_compiler
        src/main.c
        src/memory.c
        src/auxiliary.c
        src/linked_list.c
        src/stack.c


        include/kind.h
        include/memory.h
        include/auxiliary.h
        include/linked_list.h
        include/main.h
        include/stack.h

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

        src/modules/print_asm/print_asm.c
        src/modules/print_asm/include/print_asm.h

        src/modules/typechecker/typechecker.c
        src/modules/typechecker/setup.c
        src/modules/typechecker/pickup.c
        src/modules/typechecker/check.c
        src/modules/typechecker/include/typechecker.h
        src/modules/typechecker/include/setup.h
        src/modules/typechecker/include/pickup.h
        src/modules/typechecker/include/check.h


        src/modules/weeder/weeder.c
        src/modules/weeder/include/weeder.h
        )

add_executable(compiler
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
        ${SOURCE_FILES_compiler}
        )


add_executable(Z_Playground_Mark_1
        litter/playground/Mark_1/main.c
        )