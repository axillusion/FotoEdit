#ifndef IMAGEPRIV_H_INCLUDED
#define IMAGEPRIV_H_INCLUDED

#define MAX_NUM_PLANES 3

typedef struct ImgPlane {
    void* data;
    Int32 stride;
} ImgPlane;

struct Image
{
    Int32 width;
    Int32 height;
    ImgPlane planes[MAX_NUM_PLANES];
    Int8 bpp;
    Int8 format;
	IMemory* heap;
};

#endif /* IMAGEPRIV_H_INCLUDED */