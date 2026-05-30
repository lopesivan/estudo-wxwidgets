program  = main
sources  = main.cpp main.xrc.cpp
objects  = $(sources:.cpp=.o)
CXX      = $(shell wx-config --cxx)
WXRC     = wxrc
WARNINGS = -Wall
GDBFLAGS = -g
LDLIBS   = $(shell wx-config --libs)
CXXFLAGS = $(shell wx-config --cppflags) $(WARNINGS) $(GDBFLAGS)

all: $(program)

$(program): $(objects)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main.xrc.cpp: main.xrc
	$(WXRC) $< --cpp-code --output=$@ --function=InitXmlResource

clean:
	/bin/rm -rf $(program) $(objects) main.xrc.cpp tags *.orig
