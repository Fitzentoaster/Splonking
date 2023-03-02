#OBJS specifies which files to compile as part of the project
OBJS := $(wildcard src/*.cpp)

#CC specifies which compiler we're using
CC := g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS := -Iinclude/SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS := -Llib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS := -Wall -Werror -std=c++17 -Wl,-subsystem,windows 

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME := bin/splonking.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)