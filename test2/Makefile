all: main.cpp ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	g++ main.cpp -o testapp -O3 -lglut

debug: main.cpp ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	g++ main.cpp -o testapp -g -lglut

clean:
	rm testapp
