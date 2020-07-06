#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#define IMG_GRAY 1
#define IMG_RGB 2
#define IMG_YUV 3

data[][]
data**
data*

data[][][3]
data**


plane[3]

ImagePlane
{
    UInt8* data;
    
}


typedef struct Image
{
    UInt32 width;
    UInt32 height;
ImagePlane palnes[3];
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
