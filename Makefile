#Made by Troels Blicher Petersen (trpet15)

EXE = compiler
TEST = tests

SRC_DIR = src
INC_DIR = -Iinclude/
OBJ_DIR = build

MAIN_DIR = main
TEST_DIR = tests

SRC = $(filter-out src/tests.c, $(wildcard $(SRC_DIR)/*.c))
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_SRC = $(filter-out src/main.c, $(wildcard $(SRC_DIR)/*.c))
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc

CPPFLAGS += -Iinclude/							# -I is a preprocessor flag, not a compiler flag
CFLAGS += -std=c11 -Wall -Wextra -pedantic		# some warnings about bad code

LDFLAGS += -Llib								# -L is a linker flag
LDLIBS += -lm									# Left empty if no libs are needed

all: $(EXE) $(TEST)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(TEST): $(TEST_OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

clean-all:
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(RM) $(TEST)
