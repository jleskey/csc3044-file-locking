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

# Compile source files into object files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create executables
$(TARGETS): %: $(OUT_DIR)/%.o
	$(CC) $(CFLAGS) $< -o $(OUT_DIR)/$@
	rm $<

# Clean-up script
clean:
	rm -rf $(OUT_DIR)
