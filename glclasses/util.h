#pragma once
#include <algorithm>

// the max copied area is the min of the two rects
// doesn't clear the new area
static void mem_copy_2d( GLubyte *src, int srcWidth, int srcHeight,
                               GLubyte *dst, int dstWidth, int dstHeight ) {
    int maxH = std::min( srcHeight, dstHeight );
    int maxW = std::min( srcWidth, dstWidth );
    for (int h=0; h<maxH; ++h) {
        memcpy( &(dst[h * dstWidth * 4]),
                &(src[h * srcWidth * 4]),
                maxW * 4 );
    }
}


// the max copied area is the min of the two rects
// the cleared area is the new rect minus the old
static void mem_copy_2d_clear( GLubyte *src, int srcWidth, int srcHeight,
                               GLubyte *dst, int dstWidth, int dstHeight ) {
    int maxH = std::min( srcHeight, dstHeight );
    int maxW = std::min( srcWidth, dstWidth );
    int extraW = std::max( 0, dstWidth - srcWidth );
    for (int h=0; h<maxH; ++h) {
        memcpy( &(dst[h * dstWidth * 4]),
                &(src[h * srcWidth * 4]),
                maxW * 4 );
        memset( &(dst[(h * dstWidth + srcWidth) * 4]), 0, extraW * 4 );
    }
    for (int h=maxH; h<dstHeight; ++h) {
        memset( &(dst[h * dstWidth * 4]), 0, dstWidth * 4 );
    }
}

