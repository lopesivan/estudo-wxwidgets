CXX      := $(shell wx-config --cxx)
CXXFLAGS := $(shell wx-config --cxxflags) -Wall -g
LDLIBS   := $(shell wx-config --libs)

sources  := $(wildcard src/*.cpp)
objects  := $(sources:.cpp=.o)
program  := app

all: $(program)

$(program): $(objects)
	$(CXX) $(objects) -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	LD_LIBRARY_PATH=/home/linuxbrew/.linuxbrew/lib ./app

clean:
	rm -f $(program) $(objects)
