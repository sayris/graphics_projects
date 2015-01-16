# Makefile for CMSC 23700 Project 1
# Winter 2014
#

TARGET =	proj-1

SHELL =		/bin/sh
OS =		$(shell uname -s)

# the location of doxygen depends on the system that you are running,
# but you may have to edit this path.
#
ifeq ($(OS),Darwin)
  # standard location for MacLab machines
  DOXYGEN =	/Applications/Doxygen.app/Contents/Resources/doxygen
else
  DOXYGEN =	/usr/bin/doxygen
endif

BUILD_DIR =	$(shell pwd)
COMMON_DIR =	../../common
DATA_DIR =	$(BUILD_DIR)/../data

ifeq ($(OS),Darwin)
  CPPFLAGS = -I$(COMMON_DIR)/include -I/usr/local/include -I/opt/local/include
  LDFLAGS = -lglfw -framework OpenGL -framework Foundation
  CXX = clang++ -std=c++11
else
  CPPFLAGS = -I$(COMMON_DIR)/include -I/usr/local/include
  LDFLAGS = —L/usr/local/lib -lglfw -lGL -lGLU -lm
  CXX = g++
endif
CPPFLAGS +=	-DDATA_DIR=\"$(DATA_DIR)\"
#CXXFLAGS =	-O2 -Wall -pedantic
CXXFLAGS =	-g -Wall -pedantic

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
sinclude .depend
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

