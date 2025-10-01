The program simulates a hardware of a simple computer using the C lang.
The file structure follows very close to the project specifications. In the main.c file we first get the file.txt and call load_prog function from disk to parse the file line by line and store the instruction and data in main memory. A struct is used to store the opcode and data seperately. The CPU file uses a struct to contain all the CPU registers. A switch case is used to decode all the opcode instructions and execute the instructions. The header files uses extern to declare variables to allow them to be used throughout the entire filestructure. 

To compile and run the program:
A. Use the makefile:
    make
    ./main

B. Build your own makefile:
CC = gcc
CFLAGS = -g -Wall
OBJS = main.c cpu.c disk.c memory.c

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS)

clean:
	rm -f main

