#!/bin/bash

INC="-I./se8/lib"
OPTS="-std=c++11 -O3 -march=native"

g++ -o main main.cpp ${OPTS} ${INC}

