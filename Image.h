#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#define IMG_GRAY 1
#define IMG_RGB 2
#define IMG_YUV 3

typedef struct ImgPlane {
    UInt8* data;
    UInt32 stride;
} ImgPlane;

typedef struct Image
{
    UInt32 width;
    UInt32 height;
    ImgPlane planes[3];
    UInt8 bpp;
    UInt8 format;
    
} Image;

Int32 CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img );
    
void DestroyImage(
    IN OUT Image** img);
    
#endif /* IMAGE_H_INCLUDED */
