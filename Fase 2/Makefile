DIRS=build

$(shell mkdir -p $(DIRS))


all: build/engine build/generator

engine: build/engine

generator: build/generator
	
build/engine:
	g++ ./Engine/engine.cpp  ./Engine/engine.h  ./Engine/tiny* -lglut -lGL -lGLU -lXi -lXmu -o ./build/engine 

build/generator:
	g++ ./Generator/generator.cpp ./Generator/generator.h -lglut -lGL -lGLU -lXi -lXmu -o ./build/generator 

clean:
	rm ./build/engine ./build/generator
