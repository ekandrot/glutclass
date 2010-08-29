testapp: main.cu ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	nvcc main.cu -o testapp -O3 -lglut -lGLU

debug: main.cu ../glclasses/glut.h ../glclasses/pixmap.h ../glclasses/util.h
	nvcc main.cu -o debug -g -lglut -lGLU

clean:
	-rm testapp debug
