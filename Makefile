# ====== Configuration ======
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -MMD -MP

SRC_DIR = src
BUILD_DIR = build
TARGET = main

# Find all .c files under src/
SRC = $(wildcard $(SRC_DIR)/*.c)

# Replace src/ with build/ and .c with .o
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJ:.o=.d)

# ====== Build Rules ======

# Default target
$(TARGET): $(OBJ)
	@echo "Linking $@"
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Pattern rule: how to build object files in build/ from source in src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Include dependency files (.d)
-include $(DEPS)

# ====== Utility Targets ======

.PHONY: clean run

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)
