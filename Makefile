#Made by Troels Blicher Petersen (trpet15)

EXE = compiler

TEST = tests

SRC_DIR = src
INC_DIR = -Iinclude/
OBJ_DIR = build
MOD_DIR = src/modules

MAIN_DIR = main
TEST_DIR = tests

SRC = $(filter-out $(wildcard $(SRC_DIR)/scan_parse.c) $(wildcard $(SRC_DIR)/tests.c), $(wildcard $(SRC_DIR)/*.c $(MOD_DIR)/symbol_tree/*.c))
#SRC = $(wildcard $(MOD_DIR)/symbol_tree/*.c) $(SRC_DIR)/main.c $(SRC_DIR)/memory.c
#SRC = $(MOD_DIR)/symbol_tree/*.c $(SRC_DIR)/main.c $(SRC_DIR)/memory.c
#SRC = $(filter-out $(wildcard $(SRC_DIR)/scan_parse.c) $(wildcard $(SRC_DIR)/tests.c), \
#$(SRC_DIR)/main.c $(SRC_DIR)/memory.c)
#SRC = $(filter $($(SRC_DIR)/main.c) $($(SRC_DIR)/memory.c) $(wildcard $(MOD_DIR)/symbol_tree/%.c), $(wildcard $(SRC_DIR)/%))
#SRC = $(SRC_DIR)/main.c $(SRC_DIR)/memory.c $(MOD_DIR)/symbol_tree/*.c
INC = $(INC_DIR) -I$(MOD_DIR)/symbol_tree/include/

OBJRT = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ = $(OBJRT:$(MOD_DIR)/symbol_tree/%.c=$(OBJ_DIR)/modules/symbol_tree/%.o)
LIB = $(LDFLAGS) -L$(MOD_DIR)/symbol_tree/

SCANPARSE_SRC = $(filter-out $(wildcard src/main.c) $(wildcard $(SRC_DIR)/tests.c), $(wildcard $(MOD_DIR)/symbol_tree/*.c))
SCANPARSE_INC = -I$(MOD_DIR)/parser/include/ -I$(MOD_DIR)/pretty/include/
SCANPARSE_OBJ = $(TEST_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_SRC = $(filter-out $(wildcard src/main.c) $(wildcard $(SRC_DIR)/scan_parse.c), $(wildcard $(SRC_DIR)/.c))
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc

CPPFLAGS += $(INC_DIR)							# -I is a preprocessor flag, not a compiler flag
CFLAGS += -std=c11 -Wall -Wextra -pedantic		# some warnings about bad code

LDFLAGS += -Llib								# -L is a linker flag
LDLIBS += -lm									# Left empty if no libs are needed

all: $(EXE) $(TEST)

$(EXE): $(OBJ)
	$(CC) $(LIB) $(INC) $^ $(LDLIBS) -o $@
#	#mkdir $(OBJ_DIR)/compiler

$(OBJ_DIR)/modules/symbol_tree/%.o: $(MOD_DIR)/symbol_tree/%.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

$(TEST): $(TEST_OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(OBJ)
	$(RM) $(TEST_OBJ)

#clean-all:
#	$(RM) $(OBJ)
#	$(RM) $(TEST_OBJ)
#	$(RM) $(EXE)
#	$(RM) $(TEST)
