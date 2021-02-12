#ifndef IMAGEPRIV_H_INCLUDED
#define IMAGEPRIV_H_INCLUDED

#include "Image.h"
#include "Defines.h"

typedef struct ImgPlane {
    void* data;
    UInt32 stride;
} ImgPlane;

struct Image
{
    UInt32 width;
    UInt32 height;
    ImgPlane planes[MAX_NUM_PLANES];
    UInt8 bpp;
    UInt8 format;
};

#endif /* IMAGEPRIV_H_INCLUDED */