testapp: main.cpp ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	g++ main.cpp -o testapp -O3 -lglut -lGLU

debug: main.cpp ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	g++ main.cpp -o debug -g -lglut -lGLU

.PHONY: clean
clean:
	-rm testapp debug
