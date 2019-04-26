#!/bin/bash

INC="-I./lib/classes"
OPTS="-std=c++11 -O3 -march=native"

g++ -o main main.cpp ${OPTS} ${INC}

