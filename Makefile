# Название исполняемого файла
TARGET = cdb
TEST_TARGET = cdb_test

# Директории
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
TEST_DIR = tests

# Список исходных файлов
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Список объектных файлов
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Компилятор и флаги
CC = gcc
CFLAGS = -I$(INC_DIR) -Wall -Wextra -g

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# Правило для компиляции .c файлов в .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): $(filter-out obj/main.o,$(OBJS)) tests/test.c
	$(CC) -o $@ $^ $(CFLAGS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)
	rm -r $(TEST_TARGET)

# Правило для очистки
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Правило для очистки и сборки
rebuild: clean all

.PHONY: all clean rebuild
