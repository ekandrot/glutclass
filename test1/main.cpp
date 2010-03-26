#include "../glclasses/glut.h"
#include "../glclasses/pixmap.h"

class MyGlut : public Glut {
  public:
    MyGlut() : scaleX(0), scaleY(0) {}

    virtual void init( int *argc, char **argv, int width, int height ) {
        Glut::init( argc, argv, width, height );
        pixmap_.init( width, height );
    }

    virtual void render_event( void ) {
        glDrawPixels( pixmap_.width(), pixmap_.height(), GL_RGBA, GL_UNSIGNED_BYTE, pixmap_.pixels() );
    }

    virtual void mouse_event( int button, int state, int x, int y ) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            lastX = x;
            lastY = y;
        }
    }

    virtual void mouse_active_motion_event( int x, int y ) {
        int deltaX = x - lastX;
        int deltaY = y - lastY;

        scaleX += deltaX;
        scaleY += deltaY;
        do_pixels();

        lastX = x;
        lastY = y;

        glutPostRedisplay();
    }

    virtual void reshape_event( int width, int height ) {
        pixmap_.resize( width, height, CLEAR | COPY );
        do_pixels();
        Glut::reshape_event( width, height );
    }

    void do_pixels( void ) {
        for (int y=0; y<pixmap_.height(); ++y) {
            for (int x=0; x<pixmap_.width(); ++x) {
                pixmap_.pixels()[(pixmap_.width()*y + x)*4] = 0xFF;
                pixmap_.pixels()[(pixmap_.width()*y + x)*4 +1] = (y-scaleY)*(x-scaleX);
            }
        }
    }

  private:
    Pixmap  pixmap_;
    int     scaleX, scaleY;
    int     lastX, lastY; // used for moving mouse
};

int main( int argc, char **argv ) {
    MyGlut  glut;

    glut.init( &argc, argv, 1024, 768 );
    glut.do_pixels();

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
