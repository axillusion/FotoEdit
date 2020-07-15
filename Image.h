#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#define IMG_GRAY 1
#define IMG_RGB 2
#define IMG_YUV 3

#define MAX_NUM_PLANES 3

typedef struct ImgPlane {
    UInt8* data;
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

void CropImage (
    IN UInt32 height,
    IN UInt32 width,
    IN UInt32 x,
    IN UInt32 y, 
    IN OUT Image* img );

Int32 GetPlaneSize(
    IN UInt8 format,
    IN UInt32 width,
    IN UInt32 height,
    IN UInt8 plane,
    OUT UInt32* planeSize);

Int32 GetNrPlanes(
    IN UInt8 format,
    OUT UInt8* numPlanes);
    
#endif /* IMAGE_H_INCLUDED */
