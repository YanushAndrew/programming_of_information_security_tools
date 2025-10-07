CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS =

# Correctly specify source files with their paths
SRCS = main.cpp src/bbs.cpp

# Generate object file names in the same directory as sources
OBJS = $(SRCS:.cpp=.o)

EXECUTABLE = encryptor

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

# Rule to compile main.cpp
main.o: main.cpp src/bbs.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile bbs.cpp from the src directory
src/bbs.o: src/bbs.cpp src/bbs.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)

.PHONY: all clean
