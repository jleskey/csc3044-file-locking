# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -ansi

# Directories
SRC_DIR = src
OUT_DIR = out

# Files
TARGETS = filelock program1 program2

# All programs
all: $(TARGETS)

# Create output directory
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Object linking
$(TARGETS): %: $(OUT_DIR)/%

# Compile source files into object files
$(OUT_DIR)/%: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean-up script
clean:
	rm -rf $(OUT_DIR)
