# Compiler to use
CXX = g++

# Compiler flags
# -std=c++11
# -Wall
#  -g Add debug information (for dev only)
#  -I. Look for header files in the current directory
#
CXXFLAGS = -std=c++11 -Wall -g -I.

# Source files
# Check in root and utils subdirectory for .cpp
SRCS = $(wildcard *.cpp) $(wildcard utils/*.cpp)

# Object files (replaces .cpp with .o in the file list)

OBJS = $(SRCS:.cpp=.o)

# Name of the final executable program

TARGET = restaurant_reservation

# Default rule: this runs when you just type 'make'
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete: ./$(TARGET)"

# Rule to compile the source files into object files
# $< is the source file, $@ is the target object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule: deletes the executable and object files
# Run this with "make clean"
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned up build files"

# Phony targets prevent conflicts if you have files named 'all' or 'clean'
.PHONY: all clean
