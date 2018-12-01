SHELL= /bin/bash

CC=g++
CPPFILES=$(shell find . -name '*.cpp')
FLAGS=-g -O2
LIBS=-lm -lGL -lglfw -lpthread
INCLUDES=
OUTPUT=./sim

all:
	$(CC) -o $(OUTPUT) $(CPPFILES) $(LIBS) $(INCLUDES) $(FLAGS)