gxx= g++
stdopts= -std=c++11 -O3

all: main

clean:
	rm main.o
	rm main

main: main.o
	${gxx} ${stdopts} main.o -o main

main.o: main.cpp
	${gxx} ${stdopts} -c -o main.o main.cpp

