FLAGS = -Wall -Wextra -pedantic -Wvla 

release: FLAGS += -O3
release: all

debug: FLAGS += -g
debug: all

all: P1.o 
	g++ $(FLAGS) P1.o
	
P1.o: P1.cpp P1Functions.h
	g++ $(FLAGS) -c P1.cpp P1Functions.cpp
	
clean:
	rm -f P1.o
