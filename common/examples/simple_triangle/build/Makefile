# Sample Makefile for CMSC 23700 Project 1
#

TARGET =	triangle

SHELL =		/bin/sh
OS =		$(shell uname -s)

# the location of doxygen depends on the system that you are running,
# but you may have to edit this path.
#
ifeq ($(OS),Darwin)
  DOXYGEN =	/Applications/Doxygen.app/Contents/Resources/doxygen
else
  DOXYGEN =	/usr/bin/doxygen
endif

COMMON_DIR =	../../../

ifeq ($(OS),Darwin)
  CPPFLAGS=-I$(COMMON_DIR)/include -I/opt/local/include -I/usr/local/include/GLFW -I/usr/local/include/  
  LDFLAGS = -framework CoreVideo  -framework Cocoa -framework OpenGL -framework IOKit -lglfw
  CXX = clang++ -std=c++11
else
  CPPFLAGS=-I$(COMMON_DIR)/include -I/usr/local/include
  LDFLAGS = —L/usr/local/include -lglfw -lGL -lGLU -lm
  CXX = g++
endif
CXXFLAGS =	-O2 -Wall -pedantic

# where to find the source code
#
VPATH =		../src $(COMMON_DIR)/cs237-lib

SRCS =		$(wildcard ../src/*.cxx) $(wildcard $(COMMON_DIR)/cs237-lib/*.cxx)
INCLUDES =	$(wildcard ../src/*.hxx)
DOC_SRCS =	$(SRCS) $(INCLUDES) ../main-page

OBJS =		$(notdir $(SRCS:.cxx=.o))

$(TARGET):	$(OBJS) .depend
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o : %.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# include-file dependency information
#
.depend:	$(SRCS) $(INCLUDES)
	- $(CXX) $(CPPFLAGS) -MM $(SRCS) > .depend

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),devclean)
ifneq ($(MAKECMDGOALS),distclean)
sinclude .depend
endif
endif
endif

# generate documentation from sources using doxygen
#
.PHONY:		doc
doc:		../doc

../doc:		$(DOC_SRCS)
	(cd ..; $(DOXYGEN) Doxyfile)
	- touch ../doc

# cleanup by removing generated files
#
.PHONY:		clean
clean:
		rm -rf *.o $(TARGET) ../doc .depend

