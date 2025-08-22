# Use g++ as the C++ compiler
CC=g++

# Compiler flags: -c (compile only) and -Wall (show all warnings)
CFLAGS=-c -Wall

# Linker flags: use C++20 standard
LDFLAGS=-std=c++20

# List of all source code files
SOURCES=main.cpp methods.cpp

# List of all header files
HEADERS= bmp.h

# Generate object file names from source file names (main.o, methods.o)
OBJECTS=$(SOURCES:.cpp=.o)

# Name of the final executable program
EXECUTABLE=main

# Default target: build everything when just typing 'make'
all: $(SOURCES) $(EXECUTABLE)

# Rule to build the executable from object files
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@   

# Rule to compile .cpp files into .o object files
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@            

# Target to clean up build files (type 'make clean')
clean:
	rm -rf *.o $(EXECUTABLE)            