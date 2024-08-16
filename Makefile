BIN_DIR := bin
BUILD_DIR := build
SRC_DIR := src

TARGET := $(BIN_DIR)/tetris

CC := clang-18
CC_FLAGS := -g -Wall

VALGRIND := valgrind
VALGRIND_OUT := $(BIN_DIR)/valgrind-out.txt
VALGRIND_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$(VALGRIND_OUT)

OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))
HEADERS := $(wildcard $(SRC_DIR)/*.h)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(TARGET):	$(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) $(LIBS) -o $@

all:	build

.PHONY: build
build: clean $(TARGET)

valgrind:	build
	-$(VALGRIND) $(VALGRIND_FLAGS) ./$(TARGET)
	-cat $(VALGRIND_OUT) | grep 'ERROR SUMMARY'

run:	build
	./$(TARGET)

clean:
	-rm -rf $(BIN_DIR)/* $(BUILD_DIR)/* 
