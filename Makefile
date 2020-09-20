LIBZ=
CXX= g++
SRCZ= src/main.cpp
HDRZ=
INC= -I stb/ -I tinyobjloader/ -I src/
TRGT= gek.app
CUSTODEPMBUILD=

ifeq ($(OS),Windows_NT)
CUSTODEPMBUILD+= -D GLEW_STATIC -I glDeps/glew/include -I glDeps/glfw/include -I glDeps/glm -L glDeps/glfw/src -L glDeps/glew/lib
LIBZ+= -lglfw3 -lglew32 -lgdi32 -lopengl32 -static
TRGT= gek.exe
else
LIBZ+= -lGL -lglfw -lGLEW
endif

all: $(SRCZ) $(HDRZ)
	$(CXX) $(SRCZ) -o $(TRGT) -std=c++17 -O3 $(INC) $(CUSTODEPMBUILD) $(LIBZ)

run: all
	./$(TRGT)

clean:
	rm -rf $(TRGT)

rebuild: clean all

.PHONY: all clean rebuild run
.SUFFIXES:
