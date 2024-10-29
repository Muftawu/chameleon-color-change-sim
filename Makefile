CC = gcc
CFLAGS = -Wall -Wextra -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lraylib
TARGET = program
SRCS = main.c
ARGS = 
COLORS =

all: build
	
build:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)
	./$(TARGET) $(ARGS) $(COLORS)

run: build
	./$(TARGET) $(ARGS) $(COLORS)
