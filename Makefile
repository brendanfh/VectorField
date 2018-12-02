SHELL= /bin/bash

CC=g++
CPPFILES=$(shell find . -name '*.cpp')
FLAGS=-g -O2 -std=c++17
LIBS=-lm -lglfw -lpthread
INCLUDES=
DEFINES=
OUTPUT=./sim

all:
	$(CC) -o $(OUTPUT) $(CPPFILES) $(LIBS) -lGL $(INCLUDES) $(FLAGS) $(DEFINES)

apple:
	$(CC) -o $(OUTPUT) $(CPPFILES) $(LIBS) -framework OpenGL $(INCLUDES) $(FLAGS) $(DEFINES) -DAPPLE
