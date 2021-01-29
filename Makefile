OBJS = main.o engine.o mainstate.o

CC = g++

# https://software.intel.com/en-us/blogs/2012/09/26/gcc-x86-performance-hints
# optimized: = -Wall -Werror -std=c++11 -s -m64 -mfpmath=sse -Ofast -flto -march=native -funroll-loops
# debug: -g -O0
COMPILER_FLAGS = -Wall -Werror -std=c++11# -s -m64 -mfpmath=sse -Ofast -flto -march=native -funroll-loops

LINKER_FLAGS = -lSDL2 -lGL -lGLEW

OBJ_NAME = app

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

main.o : main.cpp
	$(CC) -c main.cpp $(COMPILER_FLAGS)

engine.o : engine.cpp
	$(CC) -c engine.cpp $(COMPILER_FLAGS)

mainstate.o : mainstate.cpp
	$(CC) -c mainstate.cpp $(COMPILER_FLAGS)

clean :
	rm -f $(OBJS)
