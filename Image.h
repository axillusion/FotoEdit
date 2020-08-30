#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "GenerateMovie.h"

#define IMG_GRAY 1
#define IMG_RGB 2
#define IMG_YUV420 3
#define IMG_YUV444 4

#define MAX_NUM_PLANES 3

typedef struct Rectangle
{
    Int32 top, left, width, height;
} Rectangle;

typedef struct ImgPlane {
    void* data;
    UInt32 stride;
} ImgPlane;

typedef struct Image
{
    UInt32 width;
    UInt32 height;
    ImgPlane planes[MAX_NUM_PLANES];
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

Int32 CropImage (
    IN const Image* img,
    IN UInt32 cropWidth,
    IN UInt32 cropHeight,
    IN UInt32 offsetX,
    IN UInt32 offsetY, 
    OUT Image* crop );

Int32 GetPlaneSize(
    IN UInt8 format,
    IN UInt32 width,
    IN UInt32 height,
    IN UInt8 plane,
    OUT UInt32* planeSize);

Int32 GetNrPlanes(
    IN UInt8 format,
    OUT UInt8* numPlanes);
    
Int32 GetImageSize(
    IN Image* img,
    OUT UInt32* size);

Int32 CheckImage(
    IN const Image* img,
    IN UInt32 requiredWidth,
    IN UInt32 requiredHeight,
    IN UInt8 requiredFormat);

Int32 ClearImage (
    IN OUT Image* img,
    UInt32 color );

Int32 WriteImageToFile ( 
    IN const Image* img,
    IN FILE* file );

Int32 ConvertImage ( 
    IN const Image* src,
    OUT Image* dst );

#endif /* IMAGE_H_INCLUDED */

/* 
support for format yuv444
CreateImage
DestroyImage
NumPLane
PlaneSize
Draw
Convert
Sampling - 444 ( -s )

*/