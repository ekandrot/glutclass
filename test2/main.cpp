#include "../glclasses/glut.h"

class MyGlut : public Glut {
  public:
    MyGlut() {}

    virtual void init( int *argc, char **argv, int width, int height ) {
        Glut::init( argc, argv, width, height );
    }

    virtual void mouse_passive_motion_event( int x, int y ) {
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        float   fx = (0.5f - x / (float)width()) * 5.0f;
        float   fy = (0.5f - y / (float)height()) * 5.0f;
        gluLookAt( fx, fy,5.0, fx, fy,-1.0, 0.0f,1.0f,0.0f );
        glutPostRedisplay();
    }

  private:
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
