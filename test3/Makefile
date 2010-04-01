testapp: main.cpp ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	g++ main.cpp -o testapp -O3 -lglut

debug: main.cpp ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	g++ main.cpp -o debug -g -lglut

.PHONY: clean
clean:
	rm testapp debug
