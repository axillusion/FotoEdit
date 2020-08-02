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
/*
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
*/

// VGA 640x480  aspect ratio 4:3
// HD 1920x1080 aspect ratio 6:9

/*------------------------------------------------------------------------------
 * ClearImage
 *----------------------------------------------------------------------------*/
Int32 ClearImage (
    IN OUT Image* img,
    UInt32 color )
{

    Int32 status = CheckImage ( img, img->width, img->height, IMG_GRAY );
    
    if ( status == STATUS_OK )
    {
        Int32 pos = 0;
        Int32 i;
        for ( i = 0; i < img->width * img->height; ++i ) 
        {
            //img->planes[0].data[pos] = color;
            if ( pos % img->width == 0 ) 
            {
                pos += img->planes[0].stride - img->width;
            }  
        }
    }

    return STATUS_FAIL;
}


/*------------------------------------------------------------------------------
 * CheckImage
 *----------------------------------------------------------------------------*/
Int32 CheckImage(
    IN const Image* img,
    IN UInt32 requiredWidth,
    IN UInt32 requiredHeight,
    IN UInt8 requiredFormat)
{
    Int32 status = STATUS_OK;

    if ( img == NULL ) 
    {
        printf ( "CheckImage: Unnalocated image\n" );
        status = STATUS_FAIL;
    } 

    if ( status == STATUS_OK ) 
    {
        if ( img->format != requiredFormat )
        {
            printf ( "CheckImage: Not the required format\n" );
            status = STATUS_FAIL;
        }
    }

    if ( status == STATUS_OK ) 
    {
        if ( ( img->width != requiredWidth ) || ( img->height != requiredHeight ) )
        {
            printf ( "CheckImage: Not the required dimensions\n" );
            status = STATUS_FAIL;
        }
    }

    if ( status == STATUS_OK ) 
    {
        Int32 plane;
        UInt8 planes = 0;
        GetNrPlanes ( img->format, &planes );
        for ( plane = 0; plane < planes && status == STATUS_OK; ++plane ) 
        {
            if ( img->planes[plane].data == NULL )
            {
                printf ( "CheckImage: Unallocated image plane\n" );
                status = STATUS_FAIL;
            }

        }
    }

    return status;
}

/*------------------------------------------------------------------------------
 * GetImageSize
 *----------------------------------------------------------------------------*/
Int32 GetImageSize(
    IN Image* img,
    OUT UInt32* size)
{
    Int32 status = STATUS_OK;
    if ( img == NULL || size == NULL ) 
    {
        printf ( "GetImageSize: invalid input argument" );
        status = STATUS_FAIL;
    }
    else
    {
        UInt8 planes = 0;
        UInt32 planeSize = 0;
        Int32 plane;
        GetNrPlanes ( img->format, &planes);
        size = 0;
        for ( plane = 0; plane < planes; ++plane ) 
        {
            GetPlaneSize ( img->format, img->width, img->height, plane + 1, &planeSize );
            size += planeSize;
        }
    }
    return status;
}

/*------------------------------------------------------------------------------
 * GetPlaneSize
 *----------------------------------------------------------------------------*/
Int32 GetPlaneSize(
    IN UInt8 format,
    IN UInt32 width,
    IN UInt32 height,
    IN UInt8 plane,
    OUT UInt32* planeSize)
{
    Int32 status;

    // sa tina cont de bpp
    
    if(planeSize == NULL)
    {
        printf("GetPlaneSize: invalid argument\n");
        status = STATUS_FAIL;
    }
    else if(plane == 0)
    {
        printf("GetPlaneSize: invalid plane number\n");
        status = STATUS_FAIL;
    }
    else
    {
        status = STATUS_OK;
        
        switch ( format ) 
        {
            case IMG_GRAY :
            {
                if(plane > 1)
                {
                    printf("GetPlaneSize: invalid plane number\n");
                    status = STATUS_FAIL;
                }
                else
                {
                    *planeSize = width * height;
                }
                break;
            }
            case IMG_RGB :
            {
                if(plane > 3)
                {
                    printf("GetPlaneSize: invalid plane number\n");
                    status = STATUS_FAIL;
                }
                else
                {
                    *planeSize = width * height;
                }
                break;
            }
            case IMG_YUV :
            {
                if(plane > 3)
                {
                    printf("GetPlaneSize: invalid plane number\n");
                    status = STATUS_FAIL;
                }
                else
                {
                    if(plane == 1)
                    {
                        *planeSize = width * height;
                    }
                    else
                    {
                        *planeSize = (width * height) / 4;
                    }
                }
                break;
            }
            default : 
            {
                printf("GetPlaneSize: invalid formatr\n");
                status = STATUS_FAIL;
                break;
            }
        }
    }
    
    return status;
}

/*------------------------------------------------------------------------------
 * GetNrPlanes
 *----------------------------------------------------------------------------*/
Int32 GetNrPlanes (
    IN UInt8 format,
    OUT UInt8* numPlanes)
{
    Int32 status;
    
    if(numPlanes == NULL)
    {
        status = STATUS_FAIL;
    }
    else
    {
        status = STATUS_OK;
        switch ( format ) 
        {
            case IMG_GRAY :
                *numPlanes = 1;
                break;
            case IMG_RGB :
                *numPlanes = 3;
                break;
            case IMG_YUV :
                *numPlanes = 2;
                break;
            default :
            {
                printf("GetNrPlanes: invalid format\n");
                status = STATUS_FAIL;
                *numPlanes = 0;
                break;
            }
        }
    }

    return status;
}

/*
buffer = malloc[Image, ImagePlane, Data, ]

newImg = buffer + 0;
newImg -> planes[0] = buffer + sizeof(Image);
newImg -> planes[0]->data = buffer + sizeof(Image) + sizeof(ImagePlane);
newImg -> planes[1] = newImg -> planes[0]->data + dataSize;
newImg -> planes[1]->data = newImg -> planes[1] + sizeof(ImagePlane);
...
*/


/*------------------------------------------------------------------------------
 * CreateImage
 *----------------------------------------------------------------------------*/
Int32 CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img)
{
    Int32 status;
    Image* newImg = NULL;
    UInt8 nrPlanes = 0;

    if ( img == NULL ) 
    {
        printf ( "CreateImage: invalid pointer!\n" );
        status = STATUS_FAIL;
    } 
    else
    {
        UInt32 size = (UInt32)sizeof(Image);
        newImg = malloc ( size );

        if ( newImg == NULL )
        {
            printf ( "CreateImage: Failed to allocate %u bytes", size );
            status = STATUS_FAIL;
        }
        else
        {
            memset(newImg, 0, sizeof(Image));

            newImg->width = width;
            newImg->height = height;
            newImg->format = format;
            newImg->bpp = bpp;

            status = STATUS_OK;
        }
    }

    if ( status == STATUS_OK )
    {
        status = GetNrPlanes ( format, &nrPlanes);
    }

    if ( status == STATUS_OK ) 
    {
        UInt8 plane;
        UInt32 planeSize = 0;

        for ( plane = 0; ((plane < nrPlanes) && (status == STATUS_OK)); ++plane ) 
        {
            status = GetPlaneSize(format, width, height, plane + 1, &planeSize);

            if (status == STATUS_OK)
            {
                newImg->planes[plane].data = malloc(planeSize);

                if (newImg->planes[plane].data == NULL)
                {
                    printf("CreateImage: could not allocate plane\n");
                    status = STATUS_FAIL;
                }
            }
        }
    }

    if (status == STATUS_OK)
    {
        *img = newImg;
    }
    
    return status;
}

/*------------------------------------------------------------------------------
 * DestroyImage
 *----------------------------------------------------------------------------*/
void DestroyImage (
    IN OUT Image** img ) 
{
    if (img != NULL && *img != NULL) 
    {
        Int32 plane;

        for (plane = 0; plane < MAX_NUM_PLANES; ++plane)
        {
            if ((*img)->planes[plane].data != NULL)
            {
                free((*img)->planes[plane].data);
            }
        }

        free(*img);

        *img = NULL;
    }
}

/*------------------------------------------------------------------------------
 * CropImage
 *----------------------------------------------------------------------------*/
Int32 CropImage (
    IN const Image* img,
    IN UInt32 cropWidth,
    IN UInt32 cropHeight,
    IN UInt32 offsetX,
    IN UInt32 offsetY, 
    OUT Image* crop )
{
    // vrrifica parametri de intare si iesire si intoarce status

    Int32 status = STATUS_OK;
    status = CheckImage ( img, img->width, img->height, img->format );
    if ( status == STATUS_OK )
    {
        status = CheckImage ( crop, crop->width, crop->height, crop->format );
    }

    if ( status == STATUS_OK ) 
    {
        crop->height = cropHeight;
        crop->width = cropWidth;
        crop->format = img -> format;
        crop->bpp = img -> bpp;
        crop->planes[0].stride = img -> planes[0].stride;
        crop->planes[0].data = img -> planes[0].data + offsetY * img -> planes[0].stride + offsetX;
    }
    return status;
}

/*------------------------------------------------------------------------------
 * ConvertRGBtoGray
 *----------------------------------------------------------------------------*/

Int32 Convert_RGB_to_GRAY ( 
    IN const Image* src,
    OUT Image* dst)
{
    // tine cont de stride si la dst si la src

    Int8 status = STATUS_OK;    

    status = CheckImage ( src, src->width, src->height, IMG_RGB );

    if ( status == STATUS_OK ) 
    {
        status = CheckImage ( dst, src->width, src->height, IMG_GRAY );
    }

    if ( status == STATUS_OK ) 
    {
        UInt32 indexSrc = 0;
        UInt32 indexDst = 0;
        UInt32 step;
        UInt8* r = src->planes[0].data;
        UInt8* g = src->planes[1].data;
        UInt8* b = src->planes[2].data;
        UInt8* gray = dst->planes[0].data;
        for ( step = 0; step < src->width * src->height; ++step ) 
        {
            if ( step % src->width == 0 )
            {
                indexSrc += src->planes[0].stride - src->width;
                indexDst += dst->planes[0].stride - src->width;
            }
            gray[indexDst] = ( r[indexSrc] + g[indexSrc] + b[indexSrc] ) / 3;
        }
    }

    return status;
}