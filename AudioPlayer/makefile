# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# SDL2 flags and libraries (via pkg-config)
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl2)

# Executable name
TARGET = audioPlayer

# Source files and automatic object file generation
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Delete object file after linking, without breaking incremental compilation
.INTERMEDIATE: $(OBJS)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(SDL_FLAGS)

# Compile .c -> .o (pattern rule)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the project
clean:
	rm -f $(OBJS) $(TARGET)

# Specify that all and clean are not files
.PHONY: all clean
