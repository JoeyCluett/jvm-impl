gxx= g++
obj= ./lib/obj
src= ./lib/src
inc= ./lib/inc
INC= -I${inc}
stdopts= -std=c++11 -O3 ${INC}

linkstr=  ${obj}/BinaryFileReader.o
linkstr+= ${obj}/ClassFile.o
linkstr+= ${obj}/ConstantInfo.o


all: main

clean:
	rm main.o
	rm main

main: main.o ${linkstr}
	${gxx} ${stdopts} main.o ${linkstr} -o main

main.o: main.cpp
	${gxx} ${stdopts} -c -o main.o main.cpp

${obj}/BinaryFileReader.o: ${src}/BinaryFileReader.cpp ${inc}/BinaryFileReader.h
	${gxx} ${stdopts} -c -o ${obj}/BinaryFileReader.o ${src}/BinaryFileReader.cpp

${obj}/ClassFile.o: ${src}/ClassFile.cpp ${inc}/ClassFile.h
	${gxx} ${stdopts} -c -o ${obj}/ClassFile.o ${src}/ClassFile.cpp

${obj}/ConstantInfo.o: ${src}/ConstantInfo.cpp ${inc}/ConstantInfo.h
	${gxx} ${stdopts} -c -o ${obj}/ConstantInfo.o ${src}/ConstantInfo.cpp
