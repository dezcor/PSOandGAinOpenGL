CFLAGS = -std=c++11 -ggdb

LDFLAGS = `pkg-config --static --libs glfw3` -lGL -lGLU -lGLEW
GLUTLAGS = -lGL -lGLU -lGLEW -lglut

OBJETOS = Obj

BIN = Bin

SOURSE = $(OBJETOS)/Malla.o $(OBJETOS)/main.o $(OBJETOS)/CurvaBezier.o $(OBJETOS)/Cube.o $(OBJETOS)/Enjambre.o $(OBJETOS)/GA.o $(OBJETOS)/GLTexture.o
SOURSEGLU =$(OBJETOS)/Malla.o $(OBJETOS)/mainGlut.o $(OBJETOS)/CurvaBezier.o $(OBJETOS)/Cube.o $(OBJETOS)/Enjambre.o $(OBJETOS)/GA.o $(OBJETOS)/GLTexture.o

all: $(SOURSE)
	g++ $(CFLAGS) -o $(BIN)/test $(SOURSE) $(LDFLAGS)
glut: $(SOURSEGLU)
	g++ $(CFLAGS) -o $(BIN)/main $(SOURSEGLU) $(GLUTLAGS)
$(OBJETOS)/main.o: main.cpp Camara.h CamaraMalla.h
	g++ $(CFLAGS) main.cpp -c -o $(OBJETOS)/main.o
$(OBJETOS)/Malla.o: Malla.cpp Malla.hpp
	g++ $(CFLAGS) Malla.cpp -c  -o $(OBJETOS)/Malla.o
$(OBJETOS)/CurvaBezier.o: CurvaBezier.cpp CurvaBezier.hpp
	g++ $(CFLAGS) CurvaBezier.cpp -c -o $(OBJETOS)/CurvaBezier.o
$(OBJETOS)/Cube.o: Cube.cpp Cube.hpp
	g++ $(CFLAGS) Cube.cpp -c -o $(OBJETOS)/Cube.o
$(OBJETOS)/Enjambre.o: ./PSO/Enjambre.cpp ./PSO/Enjambre.h
	g++ $(CFLAGS) ./PSO/Enjambre.cpp -c -o $(OBJETOS)/Enjambre.o
$(OBJETOS)/GA.o: GA/GA.cpp GA/GA.h
	g++ $(CFLAGS) ./GA/GA.cpp -c -o $(OBJETOS)/GA.o
$(OBJETOS)/GLTexture.o: GLTexture.cpp GLTexture.hpp
	g++ $(CFLAGS) GLTexture.cpp -c -o $(OBJETOS)/GLTexture.o
$(OBJETOS)/mainGlut.o: mainGlut.cpp mianGlut.hpp
	g++ $(CFLAGS) mainGlut.cpp -c -o $(OBJETOS)/mainGlut.o