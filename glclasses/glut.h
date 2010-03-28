#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

class Glut {
  public:
    Glut( ) : isSingle_(true), hasDepth_(false), isGameMode_(false) {}
    virtual ~Glut() {}
    
    virtual void init( int *argc, char **argv, int width, int height ) {
        static int  howMany = 0;
        ++howMany;
        if (howMany > 1)
            return;
        *theOne() = this;

        width_ = width; height_ = height;
        strcpy( windowName_, argv[0] );

        glutInit( argc, argv );
        glutInitWindowPosition( 200, 100 );
        glutInitWindowSize( width_, height_ );
        windowID_ = glutCreateWindow( windowName_ );
        glutInitDisplayMode( GLUT_RGBA
                             | (hasDepth_?GLUT_DEPTH:0) 
                             | (isSingle_?GLUT_SINGLE:GLUT_DOUBLE) );
    }

    // the following routine affect the windowing and possibly internal structures
    virtual void set_width_height( int w, int h ) {
        width_ = w;
        height_ = h;
        glutReshapeWindow( width_, height_ );
    }

    // ***************************************
    // dummy functions, to be overridden by real program functions
    //
    // for anything ending in _event, do not call the base class when overriding
    // these are just simple samples so that you know the derived class is working
    // before you go off exploring OpenGL
    // ***************************************

    // the following are called from glut's registered callbacks
    virtual void render_event( void ) {
        glClear( GL_COLOR_BUFFER_BIT
                 | (hasDepth_?GL_DEPTH_BUFFER_BIT:0) );
        glBegin(GL_TRIANGLES);
            glVertex3f(-0.5,-0.5,0.0);
            glVertex3f(0.5,0.0,0.0);
            glVertex3f(0.0,0.5,0.0);
        glEnd();
    }

    virtual void key_event( unsigned char key, int x, int y ) {
        // doesn't do any cleanup...
        if (key == 27) {
            if (isGameMode_) {
                glutLeaveGameMode();
            }
            #if defined(_WIN32)
            // couldn't figure out how to get MSVC10 to allow throw to work, so we just
            // bypass their handlers and exit, hope that they implemented good cleanup for us as well...
            exit(0);
            #endif
            
            throw 0;
        }
    }

    virtual void key_special_event( int key, int x, int y ) {
        if (key == GLUT_KEY_F1) {
            if (isGameMode_) {
                glutLeaveGameMode();
                glutSetWindow( windowID_ );
                glutShowWindow();
                isGameMode_ = false;
            } else {
                char    gamestring[256];
                sprintf( gamestring, "%dx%d:32", width_, height_ );
                glutGameModeString( gamestring );
                if (glutGameModeGet( GLUT_GAME_MODE_POSSIBLE )) {
                    glutHideWindow();
                    glutEnterGameMode();
                    isGameMode_ = true;
                    //glutFullScreen();
                }
            }
            register_callbacks();
        }
    }

    virtual void mouse_event( int button, int state, int x, int y ) {}

    virtual void mouse_active_motion_event( int x, int y ) {}

    virtual void mouse_passive_motion_event( int x, int y ) {}

    virtual void mouse_entry_event( int state ) {
        // GLUT_LEFT OR GLUT_ENTERED
    }

    virtual void reshape_event( int width, int height ) {
	// Prevent a divide by zero, when window is too short
	// (you can't make a window of zero width).
        if (height == 0)
            height = 1;

        width_ = width;
        height_ = height;
        glViewport( 0, 0, width_, height_ );

        float ratio = width_ / (float)height_;

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 45, ratio, 1, 1000 );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt( 0.0,0.0,5.0, 0.0,0.0,-1.0, 0.0f,1.0f,0.0f );
    }

    void idle_event( void ) {
    }

    // ***************************************
    // main loop - sets up the opengl context, then loops
    // ***************************************

    void register_callbacks( void ) {
        glutDisplayFunc( display_func );
        glutKeyboardFunc( key_func );
        glutSpecialFunc( key_special_func );
        glutReshapeFunc( reshape_func );
        glutIdleFunc( idle_func );

        glutMouseFunc( mouse_func );
        glutMotionFunc( mouse_active_motion_func );
        glutPassiveMotionFunc( mouse_passive_motion_func ); 
        glutEntryFunc( mouse_entry_func );
    }

    void loop( void ) {
        register_callbacks();
        if (hasDepth_)   glEnable( GL_DEPTH_TEST );
        glutMainLoop();
    }


    // ***************************************
    // static methods start here
    // ***************************************

    // there can be only one
    static Glut** theOne( void ) {
        static Glut *glut;
        return &glut;
    }

    // this function breaks the mold - it calls render, then it displays
    static void display_func( void ) {
        Glut *glut = *theOne();
        glut->render_event();
        if (glut->isSingle_)
            glFlush();
        else
            glutSwapBuffers();
    }

    static void key_func( unsigned char key, int x, int y ) {
        Glut *glut = *theOne();
        glut->key_event( key, x, glut->height_ - y );
    }

    static void key_special_func( int key, int x, int y ) {
        Glut *glut = *theOne();
        glut->key_special_event( key, x, glut->height_ - y );
    }

    static void mouse_func( int button, int state, int x, int y ) {
        Glut *glut = *theOne();
        glut->mouse_event( button, state, x, glut->height_ - y );
    }

    static void mouse_active_motion_func( int x, int y ) {
        Glut *glut = *theOne();
        glut->mouse_active_motion_event( x, glut->height_ - y );
    }

    static void mouse_passive_motion_func( int x, int y ){
        Glut *glut = *theOne();
        glut->mouse_passive_motion_event( x, glut->height_ - y );
    }

    static void mouse_entry_func( int state ){
        Glut *glut = *theOne();
        glut->mouse_entry_event( state );
    }

    static void reshape_func( int width, int height ) {
        Glut *glut = *theOne();
        glut->reshape_event( width, height );
    }

    static void idle_func( void ) {
        Glut *glut = *theOne();
        glut->idle_event();
    }

    // getters
    inline int      width( void )   {return width_;}
    inline int      height( void )  {return height_;}

    // setters
    inline void set_width( int width )   {set_width_height( width, height_ );}
    inline void set_height( int height ) {set_width_height( width_, height );}

  private:
    char    windowName_[256];
    int     windowID_;
    int     width_, height_;
    bool    isSingle_;
    bool    hasDepth_;
    bool    isGameMode_;

};


