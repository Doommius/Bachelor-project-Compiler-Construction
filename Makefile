#Made by Troels Blicher Petersen (trpet15)

EXE = compiler

TEST = test

SRC_DIR = src
INC_DIR = -Iinclude/
OBJ_DIR = build
MOD_DIR = src/modules
INC_ALL = -Iinclude/ $(INC)

MODULES = $(filter-out $(MOD_DIR)/README.md, $(wildcard $(MOD_DIR)/*))

MAIN_DIR = main
TEST_DIR = tests

INC_TEST = $(addprefix -I, $(wildcard $(MOD_DIR)/*/include/))

SRC = $(filter-out $(wildcard $(SRC_DIR)/tests.c), \
				$(wildcard $(SRC_DIR)/*.c \
				$(MOD_DIR)/*/*.c ) )
INC = 	$(INC_DIR) \
		$(INC_TEST)
OBJRT 	= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)									# | MODULES |
OBJMOD 	= $(OBJRT:$(MOD_DIR)/*/%.c=$(OBJ_DIR)/modules/*/%.o)
OBJ = $(OBJMOD)					# Symboltree
LIB = $(LDFLAGS) -L$(MOD_DIR)/symbol_tree/

TEST_SRC = $(filter-out $(wildcard src/main.c), \
				$(wildcard $(SRC_DIR)/*.c $(MOD_DIR)/symbol_tree/*.c))
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TC_SRC = $(filter-out $(wildcard src/main.c) $(wildcard $(SRC_DIR)/scan_parse.c \), \
				$(wildcard $(SRC_DIR)/*.c $(MOD_DIR)/symbol_tree/*.c))

CC = gcc

CPPFLAGS += $(INC_DIR)							# -I is a preprocessor flag, not a compiler flag
CFLAGS += -std=c11 -Wall -Wextra -pedantic -g	# some warnings about bad code

# -L is a linker flag
LDFLAGS += -Llib
# Left empty if no libs are needed
LDLIBS += -lm

all: $(EXE) $(TEST)

###
## Compiler compilation
###
$(EXE): $(OBJ_DIR)/y.tab.o $(OBJ_DIR)/lex.yy.o $(OBJ)
	$(CC) $(LIB) $(INC) $^ $(LDLIBS) -o $@ -lfl

###
## Module and object building
### 
$(OBJ_DIR)/modules/*/%.o: $(MOD_DIR)/*/%.c
	mkdir -p $(OBJ_DIR)/modules/*
	$(CC) $(INC) $(CFLAGS) -c $< -o $@


$(OBJ_DIR)/y.tab.c $(OBJ_DIR)/y.tab.h:  $(MOD_DIR)/parser/bison/exp.y
	mkdir -p $(OBJ_DIR)
	bison -y -d --report=state $(MOD_DIR)/parser/bison/exp.y
	mv y.tab.c $(OBJ_DIR)/
	cp y.tab.h $(OBJ_DIR)/
	mv y.tab.h include/

$(OBJ_DIR)/lex.yy.c: $(MOD_DIR)/scanner/flex/exp.l $(OBJ_DIR)/y.tab.h
	flex $(MOD_DIR)/scanner/flex/exp.l
	mv lex.yy.c $(OBJ_DIR)/

###
## Test program compilation
###
$(TEST): $(TEST_OBJ)
	$(CC) $(LIB) $(INC) $^ $(LDLIBS) -o $@

###
## Project files object creation
###
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(addprefix $(OBJ_DIR)/, $(MODULES:$(SRC_DIR)/%=%))
	$(CC) $(INC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)/*
	$(RM) include/y.tab.h

clean-all:
	rm -rf $(OBJ_DIR)/*
	$(RM) include/y.tab.h
	$(RM) $(EXE)
	$(RM) $(TEST)
