CC = gcc
CFLAGS = -g -Wall
OBJS = main.c cpu.c disk.c memory.c

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS)

clean:
	rm -f main