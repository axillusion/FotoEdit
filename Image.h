#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

typedef struct Image
{
    UInt32 width;
    UInt32 height;
    UInt8* data;
    UInt8 bpp;
    UInt8 format;
    
} Image;

void CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img );
    
void DestroyImage(
    IN OUT Image** img);
    
#endif /* IMAGE_H_INCLUDED */
