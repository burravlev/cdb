TARGET = cdb
TEST_TARGET = cdb_test

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
TEST_DIR = tests

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc
CFLAGS = -I$(INC_DIR) -Wall -Wextra -g

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): $(filter-out obj/main.o,$(OBJS)) tests/test.c
	$(CC) -o $@ $^ $(CFLAGS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)
	rm -r $(TEST_TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
