cmake_minimum_required(VERSION 3.6)   # CMake version check
project(Compiler)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard



include_directories(include)



add_executable(symboltable
        include/memory.h
        include/struct.h
        include/symbol.h
        src/main.c
        src/memory.c
        src/symbol.c
        )

add_executable(symboltable_test
        include/memory.h
        include/struct.h
        include/symbol.h
        src/tests.c
        src/memory.c
        src/symbol.c
)




find_package(BISON)
find_package(FLEX)


FLEX_TARGET(Scanner Scanner_And_Parser/flex/exp.l ${CMAKE_CURRENT_BINARY_DIR}/../Scanner_And_Parser/lex.yy.c)
BISON_TARGET(Parser Scanner_And_Parser/bison/exp.y ${CMAKE_CURRENT_BINARY_DIR}/../Scanner_And_Parser/y.tab.c)

ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

message( STATUS "--------------------------------------------------------------------------------------------------------------------------")
message( STATUS "Debugging information")
MESSAGE( STATUS "BISON:         " ${BISON_Parser_OUTPUTS} )
MESSAGE( STATUS "FLEX :         " ${FLEX_Scanner_OUTPUTS} )
message( STATUS "--------------------------------------------------------------------------------------------------------------------------")


add_executable(Scanner_And_Parser
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
        Scanner_And_Parser/main.c
        Scanner_And_Parser/memory.c
        Scanner_And_Parser/memory.h
        Scanner_And_Parser/pretty.c
        Scanner_And_Parser/pretty.h
        Scanner_And_Parser/tree.c
        Scanner_And_Parser/tree.h

)
