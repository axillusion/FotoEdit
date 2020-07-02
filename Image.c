#include "Macros.h"
#include "Image.h"

void CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image* img)
{
        
        img -> data = malloc ( width * height, bpp );
        img -> format = format;
        img -> bpp = bpp;
        img -> width = width;
        img -> height = height;

}



    