PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

FLAGS = -Wall -Wextra -pedantic -Wvla -std=c++11

release: FLAGS += -O3
release: all

debug: FLAGS += -g
debug: all

all: P1.o P1Functions.o
	g++ $(FLAGS) P1.o P1Functions.o -o proj1
	
P1.o: P1.cpp P1Functions.h
	g++ $(FLAGS) -c P1.cpp
	
P1Functions.o: P1Functions.cpp P1Functions.h
	g++ $(FLAGS) -c P1Functions.cpp	
	
clean:
	rm -f P1.o P1Functions.o proj1
