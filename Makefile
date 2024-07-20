CXXFLAGS = -Wall -fdiagnostics-color=always -Wno-sign-compare -Wno-format -g -O3
PYTHON_INCLUDE = C:/Users/boras/AppData/Local/Programs/Python/Python312/include
PYTHON_LIB = C:/Users/boras/AppData/Local/Programs/Python/Python312/libs
PYTHON_VERSION = 312

LDFLAGS = -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION)
INCLUDES = -I$(PYTHON_INCLUDE)
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
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)

.PHONY: clean

clean:
	del /Q main.exe