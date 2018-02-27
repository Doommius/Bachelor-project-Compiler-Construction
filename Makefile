#Made by Troels Blicher Petersen (trpet15)

EXE = compiler

SYM = symboltable

TEST = test

SCANPARSE = exp

SRC_DIR = src
INC_DIR = -Iinclude/
OBJ_DIR = build
MOD_DIR = src/modules
INC_ALL = -Iinclude/ $(INC) $(SCANPARSE_INC)

MAIN_DIR = main
TEST_DIR = tests

INC_SCANNER 	= -I$(MOD_DIR)/scanner/include/
INC_PARSER 		= -I$(MOD_DIR)/parser/include/
INC_PRETTY		= -I$(MOD_DIR)/pretty/include/
INC_SYMBOLTREE	= -I$(MOD_DIR)/symbol_tree/include/
INC_TYPECHECKER	= -I$(MOD_DIR)/typechecker/include

SRC = $(filter-out $(wildcard $(SRC_DIR)/scan_parse.c) $(wildcard $(SRC_DIR)/tests.c), \
				$(wildcard $(SRC_DIR)/*.c \
				$(MOD_DIR)/symbol_tree/*.c \
				$(MOD_DIR)/scanner/*.c   \
				$(MOD_DIR)/parser/*.c    \
				$(MOD_DIR)/pretty/*.c  ) )
INC = $(INC_DIR) $(INC_SCANNER) $(INC_PARSRER) $(INC_PRETTY) $(INC_SYMBOLTREE) $(INC_TYPECHECKER)
OBJRT = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)									# | MODULES |
OBJSC = $(OBJRT:$(MOD_DIR)/scanner/%.c=$(OBJ_DIR)/modules/scanner/%.o)			# Scanner
OBJPA = $(OBJSC:$(MOD_DIR)/parser/%.c=$(OBJ_DIR)/modules/parser/%.o)			# Parser
OBJPR = $(OBJPA:$(MOD_DIR)/pretty/%.c=$(OBJ_DIR)/modules/pretty/%.o)			# Pretty
OBJWE = $(OBJPR:$(MOD_DIR)/weeder/%.c=$(OBJ_DIR)/modules/weeder/%.o)			# Weeder
OBJSY = $(OBJWE:$(MOD_DIR)/symbol_tree/%.c=$(OBJ_DIR)/modules/symbol_tree/%.o)	# Symbol tree
OBJTY = $(OBJSY:$(MOD_DIR)/typechecker/%.c=$(OBJ_DIR)/modules/typechecker/%.o)	# Type checker
OBJRE = $(OBJTY:$(MOD_DIR)/resource/%.c=$(OBJ_DIR)/modules/resource/%.o)		# Resource
OBJCO = $(OBJRE:$(MOD_DIR)/code/%.c=$(OBJ_DIR)/modules/code/%.o)				# Code
OBJOP = $(OBJCO:$(MOD_DIR)/optimizer/%.c=$(OBJ_DIR)/modules/optimizer)			# Root
OBJ = $(OBJOP)					# Symboltree
LIB = $(LDFLAGS) -L$(MOD_DIR)/symbol_tree/

SCANPARSE_SRC = $(filter-out $(wildcard src/main.c) $(wildcard $(SRC_DIR)/tests.c), \
				$(wildcard $(SRC_DIR)/*.c \
				$(MOD_DIR)/scanner/*.c   \
				$(MOD_DIR)/parser/*.c    \
				$(MOD_DIR)/pretty/*.c ))
SCANPARSE_INC   = $(INC_DIR) -I$(MOD_DIR)/parser/include/ -I$(MOD_DIR)/pretty/include/
SCANPARSE_OBJRT = $(SCANPARSE_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)								# Root
SCANPARSE_OBJSC = $(SCANPARSE_OBJRT:$(MOD_DIR)/scanner/%.c=$(OBJ_DIR)/modules/scanner/%.o)		# Scanner
SCANPARSE_OBJPA = $(SCANPARSE_OBJSC:$(MOD_DIR)/parser/%.c=$(OBJ_DIR)/modules/parser/%.o)		# Parser
SCANPARSE_OBJ   = $(SCANPARSE_OBJPA:$(MOD_DIR)/pretty/%.c=$(OBJ_DIR)/modules/pretty/%.o)		# Pretty

TEST_SRC = $(filter-out $(wildcard src/main.c) $(wildcard $(SRC_DIR)/scan_parse.c), \
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

all: $(EXE) $(TEST) $(SCANPARSE)

###
## Compiler compilation
###
$(EXE): $(OBJ) $(OBJ_DIR)/y.tab.o $(OBJ_DIR)/lex.yy.o
	$(CC) $(LIB) $(SCANPARSE_INC) $^ $(LDLIBS) -o $@ -lfl
	$(CC) $(LIB) $(INC) $^ $(LDLIBS) -o $@

###
## Symbol tree compilation
### 

$(OBJ_DIR)/modules/symbol_tree/%.o: $(MOD_DIR)/symbol_tree/%.c
	mkdir -p $(OBJ_DIR)/modules/symbol_tree
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

###
## Scanner and Parser compilation
###
$(SCANPARSE): $(OBJ_DIR)/y.tab.o $(OBJ_DIR)/lex.yy.o $(SCANPARSE_OBJ)
	$(CC) $(LIB) $(SCANPARSE_INC) $^ $(LDLIBS) -o $@ -lfl

## Parser
$(OBJ_DIR)/modules/parser/%.o: $(MOD_DIR)/parser/%.c
	mkdir -p $(OBJ_DIR)/modules/parser
	$(CC) $(SCANPARSE_INC) $(CFLAGS) -c $< -o $@

## Pretty
$(OBJ_DIR)/modules/pretty/%.o: $(MOD_DIR)/pretty/%.c
	mkdir -p $(OBJ_DIR)/modules/pretty
	$(CC) $(SCANPARSE_INC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/y.tab.c $(OBJ_DIR)/y.tab.h:  $(MOD_DIR)/parser/bison/exp.y
	bison -y -d $(MOD_DIR)/parser/bison/exp.y
	mv y.tab.c $(OBJ_DIR)/
	cp y.tab.h $(OBJ_DIR)/
	mv y.tab.h include/

$(OBJ_DIR)/lex.yy.c: $(MOD_DIR)/scanner/flex/exp.l $(OBJ_DIR)/y.tab.h
	flex $(MOD_DIR)/scanner/flex/exp.l
	mv lex.yy.c $(OBJ_DIR)/

###
## Type checker compilation
###
$(OBJ_DIR)/modules/typechecker/%.o: $(MOD_DIR)/typechecker/%.c
	mkdir -p $(OBJ_DIR)/modules/typechecker
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

###
## Resource modules compilation
###
$(OBJ_DIR)/modules/resource/%.o: $(MOD_DIR)/resource/%.c
	mkdir -p $(OBJ_DIR)/modules/resource
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

###
## Code modules compilation
###
$(OBJ_DIR)/modules/code/%.o: $(MOD_DIR)/code/%.c
	mkdir -p $(OBJ_DIR)/modules/code
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

###
## Optimizer compilation
###
$(OBJ_DIR)/modules/optimizer/%.o: $(MOD_DIR)/optimizer/%.c
	mkdir -p $(OBJ_DIR)/modules/optimizer
	$(CC) $(INC) $(CFLAGS) -c $< -o $@
###
## Test program compilation
###
$(TEST): $(TEST_OBJ)
	$(CC) $(LIB) $(INC_ALL) $^ $(LDLIBS) -o $@

###
## Project files object creation
###
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p build
	$(CC) $(INC_ALL) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)/*
	$(RM) include/y.tab.h

clean-all:
	rm -rf $(OBJ_DIR)/*
	$(RM) include/y.tab.h
	$(RM) $(EXE)
	$(RM) $(TEST)
	$(RM) $(SCANPARSE)
