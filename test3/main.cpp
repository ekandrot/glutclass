#include <ctime>
#include <cmath>
#include "../glclasses/glut.h"

#define THIRTIETH_OF_SEC     (1.0f/30)
const double PI = 4.0*atan(1.0);

class MyGlut : public Glut {
  public:
    MyGlut() : Glut(true, true) {}

    virtual void init( int *argc, char **argv, int width, int height ) {
        Glut::init( argc, argv, width, height );
        nextTime_ = clock() + 1;    // gives us one second before we start getting reposts
        frame_ = 0;

        // a red circle that saves and restores current color
        glNewList( 1, GL_COMPILE );
        glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 1,0,0,1 );
        glBegin( GL_LINE_LOOP );
        for (int i=0; i<=360; ++i)
            glVertex2f( cosf(i*PI/180), sinf(i*PI/180) );
        glEnd();
        glPointSize( 5 );
        glBegin( GL_POINTS );
        glVertex2f( 0.0f, 0.75f );
        glEnd();
        glPopAttrib();
        glEndList();

        // an arrow
        glNewList( 2, GL_COMPILE );
        glBegin(GL_TRIANGLE_FAN);
            // head
            glVertex3f(  0.3,  0.0,   0.0 );
            for (int i=0; i<=360; ++i)
                glVertex3f( -0.2, 0.15f*cosf(i*PI/180), 0.2f*sinf(i*PI/180) );
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
            // back of head
            glVertex3f(  -0.2,  0.0,   0.0 );
            for (int i=0; i<=360; ++i)
                glVertex3f( -0.2, 0.15f*cosf(i*PI/180), 0.2f*sinf(i*PI/180) );
        glEnd();
        glBegin(GL_QUADS);
            // shaft
            for (int i=0; i<360; ++i) {
                glVertex3f( -0.7, 0.05f*cosf(i*PI/180), 0.05f*sinf(i*PI/180) );
                glVertex3f( -0.2, 0.05f*cosf(i*PI/180), 0.05f*sinf(i*PI/180) );
                glVertex3f( -0.2, 0.05f*cosf((i+1)*PI/180), 0.05f*sinf((i+1)*PI/180) );
                glVertex3f( -0.7, 0.05f*cosf((i+1)*PI/180), 0.05f*sinf((i+1)*PI/180) );
            }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
            // back of shaft
            glVertex3f(  -0.7,  0.0,   0.0 );
            for (int i=0; i<=360; ++i)
                glVertex3f( -0.7, 0.05f*cosf(i*PI/180), 0.05f*sinf(i*PI/180) );
        glEnd();
        glEndList();

        bufferDirty_ = true;
    }

    virtual void display_event( void ) {
        bufferDirty_ = true;
    }

    virtual void idle_event( void ) {
        if (clock() >= nextTime_) {
            // tell the class it is time to do the next frame
            ++frame_;
            glutPostRedisplay();
            update_nextTime( THIRTIETH_OF_SEC );
        }

        // since we are double buffered, let's fill in the back buffer
        // during idle time, then display_event will only swap and tell us
        // that the back buffer is dirty again
        if (bufferDirty_) {
            render_frame( frame_ );
            bufferDirty_ = false;
        }
    }

    void render_frame( int frame ) {
        float   angleDegrees = frame;
        float   angleRadians = frame * PI/180;
        glClear( GL_COLOR_BUFFER_BIT
                 | (hasDepth()?GL_DEPTH_BUFFER_BIT:0) );

        // let's rotate the entire rendering 1.2f degrees more on every render
        // this is a dangerous way to do it, since it means that render_frame
        // now has a hidden state!
        // if we want to remove this hidden state, move it to inside of the glPushMatrix
        // and change the 1.2f to 1.2f*angleDegrees
        glRotatef( 1.2f, 1, 1, 1 );

        // draw a circle, which isn't frame dependent
        glCallList( 1 );
        //glutStrokeCharacter( GLUT_STROKE_ROMAN, '1' );

        glPushMatrix();
        glTranslatef( cosf( angleRadians ), sinf( angleRadians ), 0.0f );
        glRotatef( angleDegrees, 0, 0, 1 );
        glCallList( 2 );
        glPopMatrix();
    }

    // this updates the next time we get called for repost.  ideally, it should end up calling
    // this code 1/seconds times per second because of the +=.  if the code were instead:
    // nextTime_ = clock() + seconds * CLOCKS_PER_SEC; then it would drift, but it would give
    // the rest of the code at least seconds to do its work.
    void update_nextTime( float seconds ) { nextTime_ += seconds * CLOCKS_PER_SEC; }

  private:
    clock_t nextTime_;
    int     frame_;
    bool    bufferDirty_;
};

int main( int argc, char **argv ) {
    MyGlut  glut;

    glut.init( &argc, argv, 480, 320 );

    int     exitCode = 0;
    try {
        glut.loop();
    }
    catch (int e) {
        exitCode = e;
    }

    return exitCode;
}
