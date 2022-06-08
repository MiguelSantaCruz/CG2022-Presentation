DIRS=build

$(shell mkdir -p $(DIRS))


all: build/engine build/generator

debug: build/engine-debug

engine: build/engine

generator: build/generator

build/engine:
	g++ ./Engine/*.cpp  ./Engine/*.h -lglut -lGL -lGLU -lXi -lXmu -lGLEW -o ./build/engine 

build/generator:
	g++ ./Generator/generator.cpp ./Generator/generator.h -lglut -lGL -lGLU -lXi -lXmu -lGLEW -o ./build/generator 

build/engine-debug:
	g++ ./Engine/*.cpp  ./Engine/*.h -g -lglut -lGL -lGLU -lXi -lXmu -lGLEW -o ./build/engine-debug 

clean:
	rm ./build/engine ./build/engine-debug ./build/generator
