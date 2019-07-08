CFLAGS = -std=c++11 -ggdb

LDFLAGS = `pkg-config --static --libs glfw3` -lGL -lGLU -lGLEW
GLUTLAGS = -lopengl32 -lglut32 -lglu32 -lglew32

OBJETOS = Obj

BIN = Bin

RES = src

SOURSE = $(OBJETOS)/Malla.o $(OBJETOS)/main.o $(OBJETOS)/CurvaBezier.o $(OBJETOS)/Cube.o $(OBJETOS)/Enjambre.o $(OBJETOS)/GA.o $(OBJETOS)/GLTexture.o
SOURSEGLU =$(OBJETOS)/Malla.o $(OBJETOS)/mainGlut.o $(OBJETOS)/CurvaBezier.o $(OBJETOS)/Cube.o $(OBJETOS)/Enjambre.o $(OBJETOS)/GA.o $(OBJETOS)/GLTexture.o

all: $(SOURSE)
	g++ $(CFLAGS) -o $(BIN)/test $(SOURSE) $(LDFLAGS)
#glut: $(SOURSEGLU)
#	g++ $(CFLAGS) -o $(BIN)/main $(SOURSEGLU) $(GLUTLAGS)
$(OBJETOS)/main.o: $(RES)/main.cpp $(RES)/Camara.h $(RES)/CamaraMalla.h
	g++ $(CFLAGS) $(RES)/main.cpp -c -o $(OBJETOS)/main.o
$(OBJETOS)/Malla.o: $(RES)/Malla.cpp $(RES)/Malla.hpp
	g++ $(CFLAGS) $(RES)/Malla.cpp -c  -o $(OBJETOS)/Malla.o
$(OBJETOS)/CurvaBezier.o: $(RES)/CurvaBezier.cpp $(RES)/CurvaBezier.hpp
	g++ $(CFLAGS) $(RES)/CurvaBezier.cpp -c -o $(OBJETOS)/CurvaBezier.o
$(OBJETOS)/Cube.o: $(RES)/Cube.cpp $(RES)/Cube.hpp
	g++ $(CFLAGS) $(RES)/Cube.cpp -c -o $(OBJETOS)/Cube.o
$(OBJETOS)/Enjambre.o: ./PSO/Enjambre.cpp ./PSO/Enjambre.h
	g++ $(CFLAGS) ./PSO/Enjambre.cpp -c -o $(OBJETOS)/Enjambre.o
$(OBJETOS)/GA.o: GA/GA.cpp GA/GA.h
	g++ $(CFLAGS) ./GA/GA.cpp -c -o $(OBJETOS)/GA.o
$(OBJETOS)/GLTexture.o: $(RES)/GLTexture.cpp $(RES)/GLTexture.hpp
	g++ $(CFLAGS) $(RES)/GLTexture.cpp -c -o $(OBJETOS)/GLTexture.o
$(OBJETOS)/mainGlut.o: $(RES)/mainGlut.cpp $(RES)/mianGlut.hpp
	g++ $(CFLAGS) $(RES)/mainGlut.cpp -c -o $(OBJETOS)/mainGlut.o
