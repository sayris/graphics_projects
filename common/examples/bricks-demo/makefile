# Makefile for Bricks demo; CMSC 23700
# Winter 2014
#

TARGET =	bricks

SHELL =		/bin/sh
OS =		$(shell uname -s)

BUILD_DIR =	$(shell pwd)
COMMON_DIR =	../..
DATA_DIR =	$(BUILD_DIR)

ifeq ($(OS),Darwin)
  CPPFLAGS = -I$(COMMON_DIR)/include -I/usr/local/include -I/opt/local/include
  LDFLAGS = -L/opt/local/lib -lglfw -lpng -framework OpenGL -framework Foundation
  CXX = clang++ -std=c++11
else
  CPPFLAGS = -I$(COMMON_DIR)/include -I/usr/local/include
  LDFLAGS = —L/usr/local/include -lglfw -lpng -lGL -lGLU -lm
  CXX = g++
endif
CPPFLAGS +=	-DDATA_DIR="\"$(DATA_DIR)\""
#CXXFLAGS =	-O2 -Wall -pedantic
CXXFLAGS =	-g -Wall -pedantic

# where to find the source code
#
VPATH =		. $(COMMON_DIR)/cs237-lib

SRCS =		main.cxx $(wildcard $(COMMON_DIR)/cs237-lib/*.cxx)

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

# cleanup by removing generated files
#
.PHONY:		clean
clean:
		rm -rf *.o $(TARGET) ../doc .depend

