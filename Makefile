#	Fred Smalley
#	independent makefile
#	created:	12-24-2014
#

# Set Environment
PROJECT_ROOT := $(CURDIR)

# Variables
TARGET = dbSNPMod
OBJS := $(patsubst src/%.cpp, %.o, $(wildcard src/*.cpp))
CXX = g++
CXXFLAGS = -std=c++11 -I $(PROJECT_ROOT)/include

# Default rule
all:	env $(TARGET)

env:
	echo $(PROJECT_ROOT)

$(TARGET) :	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS)

# Rules
%.o :		src/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm $(TARGET) $(OBJS)
