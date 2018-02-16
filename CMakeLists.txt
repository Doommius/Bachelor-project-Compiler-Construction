cmake_minimum_required(VERSION 3.6)   # CMake version check
project(Compiler)                     # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

set(SOURCE_FILES_symboltable
        symboltree/include/memory.h
        symboltree/include/struct.h
        symboltree/include/symbol.h
        symboltree/src/memory.c
        symboltree/src/symbol.c)

add_executable(symboltable
        symboltree/src/main.c
        ${SOURCE_FILES_symboltable}
        )

add_executable(symboltable_test
        symboltree/src/tests.c
        ${SOURCE_FILES_symboltable}
)

find_package(BISON)
find_package(FLEX)

FLEX_TARGET(Scanner Scanner_And_Parser/flex/exp.l ${CMAKE_CURRENT_BINARY_DIR}/../Scanner_And_Parser/lex.yy.c)


#Bison Flags -v -r all usefull for debugging
BISON_TARGET(Parser Scanner_And_Parser/bison/exp.y ${CMAKE_CURRENT_BINARY_DIR}/../Scanner_And_Parser/y.tab.c)

ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)


set(SOURCE_FILES_scanner_And_parser
         Scanner_And_Parser/main.c
         Scanner_And_Parser/memory.c
         Scanner_And_Parser/memory.h
         Scanner_And_Parser/pretty.c
         Scanner_And_Parser/pretty.h
         Scanner_And_Parser/tree.c
         Scanner_And_Parser/tree.h)

add_executable(Scanner_And_Parser
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
        ${SOURCE_FILES_scanner_And_parser}

)


add_executable(Z_Playground_Mark_1
        Playground/Mark_1/main.c
        Playground/Mark_1/main.h
        )