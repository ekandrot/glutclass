#include </usr/local/cuda/include/cuda_runtime.h>
#include "../glclasses/glut.h"
#include "../glclasses/pixmap.h"


// ***************************  CUDA kernel ***********************************************
__global__ void cuda_kernel( int scaleX, int scaleY,
                             int width, int height,
                             int stride, GLubyte *devPixmap ) {
    // first load up the bounds, and make sure we are within it
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    if (x >= width) return;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    if (y >= height) return;

    // then fill in the pixel values
    devPixmap[(stride * y + x)*4] = 0xFF;
    devPixmap[(stride * y + x)*4 +1] = (y - scaleY) * (x - scaleX);
    devPixmap[(stride * y + x)*4 +2] = 0;
    devPixmap[(stride * y + x)*4 +3] = 0xFF;
}

// ***************************  MyGlut class ***********************************************
class MyGlut : public Glut {
  public:
    MyGlut() : scaleX(0), scaleY(0) {}
    ~MyGlut() {
        cudaFree( devPixmap_ );
    }

    virtual void init( int *argc, char **argv, int width, int height ) {
        Glut::init( argc, argv, width, height );
        pixmap_.init( width, height );
        cudaMalloc( (void**)&devPixmap_, pixmap_.bytes() );
    }

    virtual void display_event( void ) {
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
        cudaFree( (void*)devPixmap_ );
        cudaMalloc( (void**)&devPixmap_, pixmap_.bytes() );
        do_pixels();
        Glut::reshape_event( width, height );
    }

    void do_pixels( void ) {
        dim3    grids((pixmap_.width()+15)/16,(pixmap_.height()+15)/16);
        dim3    threads(16,16);
        cuda_kernel<<<grids,threads>>>( scaleX, scaleY,
                                        pixmap_.width(), pixmap_.height(),
                                        pixmap_.stride(), devPixmap_ );
        cudaMemcpy( pixmap_.pixels(), devPixmap_, pixmap_.bytes(), cudaMemcpyDeviceToHost );
    }

  private:
    Pixmap  pixmap_;
    GLubyte *devPixmap_;
    int     scaleX, scaleY;
    int     lastX, lastY; // used for moving mouse
};

int main( int argc, char **argv ) {
    MyGlut  glut;

    glut.init( &argc, argv, 1024, 768 );
    glut.do_pixels();

    int     exitCode = 0;
    try {
        glut.loop();
    }
    catch (int e) {
        exitCode = e;
    }

    return exitCode;
}
