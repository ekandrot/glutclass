#include "../glclasses/glut.h"

class MyGlut : public Glut {
  public:
    MyGlut() {}
    ~MyGlut() {}
};

int main( int argc, char **argv ) {
    MyGlut  glut;

    glut.init( &argc, argv, 1024, 768 );

    // we use try/catch to clean up after glut's endless loop
    // in the glut or derived glut classes, we should throw rather than exit
    int     exitCode = 0;
    try {
        glut.loop();
    }
    catch (int e) {
        exitCode = e;
    }

    return exitCode;
}
