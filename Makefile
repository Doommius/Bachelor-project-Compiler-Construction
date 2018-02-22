#Made by Troels Blicher Petersen (trpet15)

EXE = compiler

TEST = test

SCANPARSE = exp

SRC_DIR = src
INC_DIR = -Iinclude/
OBJ_DIR = build
MOD_DIR = src/modules
INC_ALL = -Iinclude/ $(INC) $(SCANPARSE_INC)

MAIN_DIR = main
TEST_DIR = tests

SRC = $(filter-out $(wildcard $(SRC_DIR)/scan_parse.c) $(wildcard $(SRC_DIR)/tests.c), \
				$(wildcard $(SRC_DIR)/*.c $(MOD_DIR)/symbol_tree/*.c) )
INC = $(INC_DIR) -I$(MOD_DIR)/symbol_tree/include/
OBJRT = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)													# Root
OBJ = $(OBJRT:$(MOD_DIR)/symbol_tree/%.c=$(OBJ_DIR)/modules/symbol_tree/%.o)					# Symboltree
LIB = $(LDFLAGS) -L$(MOD_DIR)/symbol_tree/

SCANPARSE_SRC = $(filter-out $(wildcard src/main.c) $(wildcard $(SRC_DIR)/tests.c), \
				$(wildcard $(SRC_DIR)/*.c \
				$(MOD_DIR)/scanner/*.c)   \
				$(MOD_DIR)/parser/*.c)    \
				$(MOD_DIR)/pretty/*.c
SCANPARSE_INC   = $(INC_DIR) -I$(MOD_DIR)/parser/include/ -I$(MOD_DIR)/pretty/include/
SCANPARSE_OBJRT = $(SCANPARSE_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)								# Root
SCANPARSE_OBJSC = $(SCANPARSE_OBJRT:$(MOD_DIR)/scanner/%.c=$(OBJ_DIR)/modules/scanner/%.o)		# Scanner
SCANPARSE_OBJPA = $(SCANPARSE_OBJSC:$(MOD_DIR)/parser/%.c=$(OBJ_DIR)/modules/parser/%.o)		# Parser
SCANPARSE_OBJ   = $(SCANPARSE_OBJPA:$(MOD_DIR)/pretty/%.c=$(OBJ_DIR)/modules/pretty/%.o)		# Pretty

TEST_SRC = $(filter-out $(wildcard src/main.c) $(wildcard $(SRC_DIR)/scan_parse.c), \
				$(wildcard $(SRC_DIR)/*.c $(MOD_DIR)/symbol_tree/*.c))
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc

CPPFLAGS += $(INC_DIR)							# -I is a preprocessor flag, not a compiler flag
CFLAGS += -std=c11 -Wall -Wextra -pedantic		# some warnings about bad code

# -L is a linker flag
LDFLAGS += -Llib
# Left empty if no libs are needed
LDLIBS += -lm

all: $(EXE) $(TEST) $(SCANPARSE)

###
## Compiler compilation
###
$(EXE): $(OBJ)
	$(CC) $(LIB) $(INC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/modules/symbol_tree/%.o: $(MOD_DIR)/symbol_tree/%.c
	mkdir -p $(OBJ_DIR)/modules/symbol_tree
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

###
## Scanner and Parser compilation
###
$(SCANPARSE): $(OBJ_DIR)/y.tab.o $(OBJ_DIR)/lex.yy.o $(SCANPARSE_OBJ)
	$(CC) $(LIB) $(SCANPARSE_INC) $^ $(LDLIBS) -o $@ -lfl

## Scanner
#$(OBJ_DIR)/modules/scanner/flex/: $(MOD_DIR)/scanner/flex/%.l
#	mkdir -p $(OBJ_DIR)/modules/scanner


## Parser
$(OBJ_DIR)/modules/parser/%.o: $(MOD_DIR)/parser/%.c
	mkdir -p $(OBJ_DIR)/modules/parser
	$(CC) $(SCANPARSE_INC) $(CFLAGS) -c $< -o $@

## Pretty
$(OBJ_DIR)/modules/pretty/%.o: $(MOD_DIR)/pretty/%.c
	mkdir -p $(OBJ_DIR)/modules/pretty
	$(CC) $(SCANPARSE_INC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/y.tab.c $(MOD_DIR)/y.tab.h:  $(MOD_DIR)/parser/bison/exp.y
	bison -y -d $(MOD_DIR)/parser/bison/exp.y
	mv y.tab.c $(OBJ_DIR)/
	cp y.tab.h $(OBJ_DIR)/
	mv y.tab.h include/

$(OBJ_DIR)/lex.yy.c: $(MOD_DIR)/scanner/flex/exp.l $(OBJ_DIR)/y.tab.h
	flex $<
	mv lex.yy.c $(OBJ_DIR)/

###
## Test program compilation
###
$(TEST): $(TEST_OBJ)
	$(CC) $(LIB) $(INC_ALL) $^ $(LDLIBS) -o $@

###
## Project files object creation
###
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(INC_ALL) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)/*
	$(RM) include/y.tab.h

clean-all:
	rm -rf $(OBJ_DIR)/*
	$(RM) $(EXE)
	$(RM) $(TEST)
	$(RM) $(SCANPARSE)
