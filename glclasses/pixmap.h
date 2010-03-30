#pragma once
#include <iostream>
#include "util.h"



// flags that can be or'ed together, for resizing
enum ResizeFlags {
    NONE    = 0,
    CLEAR   = 0x01,
    COPY    = 0x02,
};
inline ResizeFlags operator|(ResizeFlags a, ResizeFlags b) {
    return static_cast<ResizeFlags>((int)a | (int)b);
}

class Pixmap {
  public:
    Pixmap() {}

    ~Pixmap( void ) {
        delete [] pixels_;
    }

    void init( int width, int height ) {
        width_ = width;
        height_ = height;
        pixels_ = new GLubyte [width_ * height_ * 4];
    }

    void resize( int width, int height, ResizeFlags flags ) {
        // create a new pixel area
        GLubyte *pixels = new GLubyte [width * height * 4];
        if (pixels == NULL) {
            std::cerr << "Couldn't allocate pixmap with new size" << std::endl;
            return;
        }

        // clear the new pixel buffer, then copy over as much of the old one as possible
        if (flags & CLEAR)
            memset( pixels, 0, width * height * 4 );
        if (flags & COPY)
            mem_copy_2d( pixels_, width_, height_, pixels, width, height );

        // set object's internals to new values
        delete [] pixels_;
        pixels_ = pixels;
        width_ = width;
        height_ = height;
    }

    // getters
    inline GLubyte *pixels( void )  {return pixels_;}
    inline int      width( void )   {return width_;}
    inline int      height( void )  {return height_;}
    inline int      stride( void )  {return width_;}

    // setters
    inline void set_width( int width )   {resize( width, height_, CLEAR | COPY );}
    inline void set_height( int height ) {resize( width_, height, CLEAR | COPY );}


  private:
    GLubyte *pixels_;
    int     width_, height_;
};

