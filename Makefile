# 

CXX		= g++
CFLAGS		= -Wall -std=c++11
OFLAGS		= -O2 -fopenmp

# files

OBJ		= obj/main.o obj/tools.o obj/tsp.o obj/graph.o
TGT		= bin/main

# first rule

all:	$(TGT)
	@echo "building done."

# linking

$(TGT):	$(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(OFLAGS)

# compiling

obj/%.o:	src/%.cpp
	$(CXX) -c $(CFLAGS) $(OFLAGS) -o $@ $<

# cleaning

clean: $(OBJ) $(TGT)
	rm $^
