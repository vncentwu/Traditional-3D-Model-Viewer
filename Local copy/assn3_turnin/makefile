main: main.o
	g++ -o main main.o -lGL -lGLU -lglut -lXext -lX11 -L/usr/lib/nvidia-340 -lpthread

main.o: main.cpp loader.h geom.h
	g++ -c main.cpp -lGL -lGLU -lglut -lXext -lX11 -L/usr/lib/nvidia-340 -lpthread

clean:
	$(RM) main



