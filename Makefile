CXX = g++
CXXFLAGS = -Iinclude -DTINYGLTF3_ENABLE_FS
LDFLAGS = -lSDL3 -lGL

SRC = src/main.cpp src/glad.c utils/utils.cpp include/tinygltf/tiny_gltf_v3.c
OUT = app

all:
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(OUT)

run: all
	./$(OUT)
