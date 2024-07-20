CXXFLAGS = -Wall -fdiagnostics-color=always -Wno-sign-compare -Wno-format \
            -L"C:\Users\boras\AppData\Local\Programs\Python\Python312\libs" \
            -lpython312 \
            -I"C:\Users\boras\AppData\Local\Programs\Python\Python312\include" \
            -g -O3

# Directories
SRCDIR = .
INCDIR = include
BINDIR = .

# Source files
SOURCES = $(wildcard *.cpp) \
          $(wildcard Solvers/2-SAT/*.cpp) \
          $(wildcard Solvers/Horn/*.cpp) \
          $(wildcard Solvers/Nested/*.cpp) \
          $(wildcard Solvers/Co-Nested/*.cpp)
EXECUTABLE = main

# Build target
all: $(EXECUTABLE)

# Rule to create executable
$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

.PHONY: clean

clean:
	del /Q main.exe