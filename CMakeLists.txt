cmake_minimum_required(VERSION 3.6)   # CMake version check
project(Compiler)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard



include_directories(include)


FIND_PACKAGE(BISON REQUIRED)
SET(BisonOutput ${CMAKE_SOURCE_DIR}/Scanner_And_Parser/y.tab.c)
IF(BISON_FOUND)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${BisonOutput}
      COMMAND ${BISON_EXECUTABLE}
              --defines=${CMAKE_SOURCE_DIR}/Scanner_And_Parser/y.tab.h
              --output=${BisonOutput}
              ${CMAKE_SOURCE_DIR}/Scanner_And_Parser/exp.y
      COMMENT "Generating parser.cpp"
    )
ENDIF()

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput ${CMAKE_SOURCE_DIR}/Scanner_And_Parser//lex.yy.c)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${FlexOutput}
      COMMAND ${FLEX_EXECUTABLE}
              --outfile=${FlexOutput}
              ${CMAKE_SOURCE_DIR}/Scanner_And_Parser/exp.l
      COMMENT "Generating scanner.cpp"
    )
ENDIF()




ADD_LIBRARY(LibFlex ${FlexOutput})
ADD_LIBRARY(LibBison ${BisonOutput})
ADD_LIBRARY(LibAll ${BisonOutput} ${FlexOutput})


add_executable(Compiler
        include/memory.h
        include/struct.h
        include/symbol.h
        src/main.c
        src/memory.c
        src/symbol.c
)

add_executable(tests
        include/memory.h
        include/struct.h
        include/symbol.h
        src/tests.c
        src/memory.c
        src/symbol.c
)

add_executable(Scanner_And_Parser
    Scanner_And_Parser/main.c
    Scanner_And_Parser/exp.y
    Scanner_And_Parser/exp.l
    Scanner_And_Parser/memory.c
    Scanner_And_Parser/memory.h
    Scanner_And_Parser/pretty.c
    Scanner_And_Parser/pretty.h
    Scanner_And_Parser/tree.c
    Scanner_And_Parser/tree.h
)


target_link_libraries(Scanner_And_Parser MyLib)
