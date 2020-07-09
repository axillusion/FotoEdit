#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Macros.h"
#include "Image.h"

/*
YUV 420 planar
+---+---+---+---+---+---+---+---+
| y | y | y | y | u | u | v | v |
+---+---+---+---+---|---+---+---+
| y | y | y | y | u | u | v | v |
+---+---+---+---+---+---+---+---+
| y | y | y | y |
+---+---+---+---+
| y | y | y | y |
+---+---+---+---+

datasize = width * height * 3 / 2;
*/

stride = width + 1

UInt8* data = malloc( stride * height)



Image.width = width
Image.height = height

3x3

0x00, 0x01, 0x02, 0x03
0x03, 0x04, 0x05, 0x07
0x06, 0x07, 0x08, 0x0B

+---+---+---+---+
| 0 | 1 | 2 | 0 |
+---+---+---+---+
| 3 | 4 | 5 | 0 |
+---+---+---+---+
| 6 | 7 | 8 | 0 |
+---+---+---+---+



Image crop
4x4
2x2 crop 
offset x = 1
offset y = 2 

+----+----+----+----+----+----+----+----+
| Y1 | Y1 | Y2 | Y2 | u1 | u2 | v1 | v2 |
+----+----+----+----+----+----+----+----+
| Y1 | Y1 | Y2 | Y2 | u3 | u4 | v3 | v4 |
+----+----+----+----+----+----+----+----+
| Y3 | Y3 | Y4 | Y4 |
+----+----+----+----+
| Y3 | Y3 | Y4 | Y4 |
+----+----+----+----+

UInt8 imgData = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];


Image img;
img.width = 4;
img.height = 4;
img.format = 0;
img.bpp = 8;
img.planes[0].data = imgData;
img.planes[0].stride = 4;

Image crop;
crop.width = 2;
crop.height = 2;
crop.format = img.format;
crop.bpp = img.bpp;
crop.planes[0].stride = img.planes[0].stride;
crop.planes[0].data = img.planes[0].data + y * img.planes[0].stride + x;




Int32 CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img)
{
    Int32 status = STATUS_OK;

    if ( img == NULL ) 
    {
        printf ( "CreateImage: invalid pointer!\n" );
        status = STATUS_FAIL;
    } 
    else 
    {
        Image* newImg;

        newImg = malloc ( sizeof ( Image ) );
        if ( newImg == NULL ) 
        {
            printf ( "CreateImage: could not allocate %d bytes\n", sizeof ( Image ) );
            status = STATUS_FAIL;
        }
        else 
        {
            UInt32 dataSize, nrPlanes;
            
            dataSize = width * height * ( ( bpp + 7) >> 3 );

            switch ( format ) 
            {
                case IMG_GRAY : 
                    nrPlanes = 1;
                    break;
                case IMG_RGB :
                    nrPlanes = 3;
                    break;
                case IMG_YUV :
                    nrPlanes = 2;
                    break;
                default :
                    dataSize = 0;
                    nrPlanes = 0;
                    printf ( "CreateImage: Unknown format %d", format );
                    status = STATUS_FAIL;  
                    break;
            }
            if ( status == STATUS_OK )
            {
                memset ( newImg, 0, sizeof ( Image ) );

                int i;
                for ( i = 0; i < nrPlanes; ++i )
                    newImg -> planes[i] -> data = malloc ( dataSize );
                
                if ( newImg -> planes == NULL ) 
                {
                    printf ( "CreateImage: could not allocate %d bytes\n", dataSize );
                    status = STATUS_FAIL;
                }
                else 
                {   
                    memset ( newImg -> planes, 0, dataSize );
                    newImg -> format = format;
                    newImg -> bpp = bpp;
                    newImg -> width = width;
                    newImg -> height = height;
                    *img = newImg;

                    //status = STATUS_OK;
                }
            }
        }
    }

    return status;
}

void DestroyImage (
    IN OUT Image** img ) 
{
    if ( img != NULL ) 
    {   
        free ( ( *img )-> planes ) ;
        free ( *img );
    }
}

    