CXXFLAGS = -Wall -fdiagnostics-color=always -Wno-sign-compare -Wno-format -g -O3

# Directories
SRCDIR = .
INCDIR = include
BINDIR = .

# Source files
SOURCES = $(wildcard *.cpp) \
          $(wildcard Solvers/2-SAT/*.cpp) \
          $(wildcard Solvers/Horn/*.cpp) \
          $(wildcard Solvers/Nested/*.cpp)
EXECUTABLE = main

# Build target
all: $(EXECUTABLE)

# Rule to create executable
$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

.PHONY: clean

clean:
	del /Q *.o Solvers\2-SAT\*.o Solvers\Horn\*.o Solvers\Nested\*.o main.exe