# Build
CPP      = g++
DEBUG    = -g

all: build run

build: src/main.cpp src/reader.cpp src/genetic.cpp
	$(CPP) src/main.cpp src/reader.cpp src/genetic.cpp -o bin/exec

run: 
	./bin/exec

print: 
	./bin/exec > print.txt

debug: src/main.cpp src/reader.cpp src/genetic.cpp
	$(CPP) $(DEBUG) src/main.cpp src/reader.cpp src/genetic.cpp -o bin/exec