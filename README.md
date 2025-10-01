### About the Project 

The program simulates a hardware of a simple computer using the C lang.
The file structure follows very close to the project specifications. In the main.c file we first get the file.txt and call load_prog() function from disk to parse the file line by line and store the instruction and data in main memory. In the disk file, a struct is used to automatically store the opcode and data seperately from the initial string. In memory write, the opcode and argument are properly stored using pointers. Memory write returns an array with an index of 2. The CPU file uses a struct to contain all the CPU registers. A switch case is used to decode all the opcode instructions and execute the instructions. The header files uses extern to declare variables to allow them to be used throughout the entire file structure. 

#### To compile and run the program:

A. Use the makefile:
```
make clean && make
./main
```
B. Run manually:
```
rm -f main
gcc -g -Wall -o main main.c cpu.c disk.c memory.c
./main.c
```