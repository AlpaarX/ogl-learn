CXX = g++
CXXFLAGS = -Iinclude
LDFLAGS = -lSDL3 -lGL

SRC = src/main.cpp src/glad.c
OUT = app

all:
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(OUT)

run: all
	./$(OUT)
