# Report

## Structure and programs

2 programs

1. C program given in the pdf
2. Java program has 2 files Sqr (directory 3) contains function to square an int and Caller (directory 4) which uses Sqr as a library and prints the output of square function applied on commandline input

makefile in each subdirectory compiles corresponding object files for c and class files for java

The makefile in Top makes corresponding executables and runs it 

## Commands in Top's makefile

- run: compiles both programs and executes them

- all: compiles both programs executable for myApp in exe and executable for Caller in obj (Java requires the linked class files to be in same directory)

- exe/myApp: compiles myApp
  
- clean: removes obj and exe folder, no files are created out of these folders

- dir: creates obj and exe folder


### Python embedding
I extracted python's library path for linking and loading by using the shell commands - `whereis, sed, grep, tail`

1. Thereis locates the binary, source, and manual page files for a command I use it to locate installed versions of python
2. Sed and Grep to seperate out the location that contains word "include". 
3. Tail to select the latest version as its arranged lexicographically
4. -I with the obtained string for loading of libraries
5. Seperate out python+version string (like- python3.8). -L with obtained string for linking of libraries

### Recursive calls

The recursive call are done by manually added `make -C subdirectory` recipe and not through bash loop.