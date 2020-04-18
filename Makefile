LIBZ= -lGL -lglfw -lGLEW
CXX= g++
SRCZ= src/main.cpp
HDRZ=
TRGT= pdoomu.app

all: $(SRCZ) $(HDRZ)
	$(CXX) $(SRCZ) -o $(TRGT) -std=c++17 -g -I src/ $(LIBZ)

clean:
	rm -rf $(TRGT)

rebuild: clean all

.PHONY: all clean rebuild
.SUFFIXES: