testapp: main.cu ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	nvcc main.cu -o testapp -O3 -lglut

debug: main.cu ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	nvcc main.cu -o debug -g -lglut

clean:
	-rm testapp debug
