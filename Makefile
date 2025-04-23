# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra

# Directories
SRC_DIR = src
OUT_DIR = out

# Files
TARGETS = filelock program1 program2

# All programs
all: $(TARGETS)

# Create the object file directory.
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Target object files.
$(TARGETS): %: $(OUT_DIR)/%

# Compile source files into object files.
$(OUT_DIR)/%: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Clean-up script
clean:
	rm -rf $(OUT_DIR)
