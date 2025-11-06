sources        = $(wildcard *.cpp)
program        = $(sources:.cpp=)
objects        = $(sources:.cpp=.o)

##############################################################################
##############################################################################
##############################################################################

CXX         = `wx-config --cxx`
WARNINGS    = -Wall
GDBFLAGS    = -g
LDLIBS      = `wx-config --libs`

CPPFLAGS    = `wx-config --cppflags` $(WARNINGS) $(GDBFLAGS)
#$(INCLUDE) $(DEFINEFLAGS)

all:          $(program)

$(program):

clean:
	/bin/rm -rf $(program) $(objects) *.orig
