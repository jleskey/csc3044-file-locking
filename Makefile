# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -ansi

# Directories
SRC_DIR = src
OUT_DIR = out

# Files
SRCS = $(SRC_DIR)/filelock.c
TARGET = $(OUT_DIR)/filelock

# Program binary
all: $(TARGET)

# Object linking
$(TARGET): $(SRCS)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean-up script
clean:
	rm -rf $(OUT_DIR)
