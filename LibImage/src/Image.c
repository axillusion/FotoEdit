#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "LibImage.h"
#include "ImagePriv.h"

Int32 Image_GetWidth ( 
    IN const Image *img )
{
    Int32 width = -1;

    if ( img != NULL )
    {
        width = img->width;
    }

    return width;
}

Int32 Image_GetHeight ( 
    IN const Image *img )
{
    UInt32 height = -1;

    if ( img != NULL )
    {
        height = img->height;
    }

    return height;
}

Int8 Image_GetBPP ( 
    IN const Image *img )
{
    UInt8 bpp = -1;

    if ( img != NULL )
    {
        bpp = img->bpp;
    }

    return bpp;
}

Int8 Image_GetFormat ( 
    IN const Image *img )
{
    UInt8 format = -1;

    if ( img != NULL )
    {
        format = img->format;
    }

    return format;
}

Int8 Image_GetNumPlanes (
    IN const Image *img )
{
	Int8 numPlanes = -1;

	if ( img != NULL )
	{
		switch ( img->format ) 
		{
			case IMG_GRAY :
				numPlanes = 1;
				break;
			case IMG_RGB :
				numPlanes = 3;
				break;
			case IMG_YUV420 :
				numPlanes = 3;
				break;
			case IMG_YUV444 :
				numPlanes = 3;
				break;
		}
	}

    return numPlanes;
}

Int32 Image_GetPlaneStride (
    IN const Image *img,
    IN const UInt8 plane )
{
	UInt32 stride = -1;

	if ( img != NULL )
	{
		stride = img->planes[plane].stride;
	}
	
	return stride;
}

void* Image_GetPlaneData (
    IN const Image *img,
    IN const UInt8 plane )
{
	void* data = NULL;

	if ( img != NULL )
	{
		data = img->planes[plane].data;
	}
	
	return data;
}

/*------------------------------------------------------------------------------
 * Image_Clear
 *----------------------------------------------------------------------------*/
Int32 Image_Clear (
    IN OUT Image* img,
    UInt32 color )
{
    
    Int32 status = Image_Check ( img, img->width, img->height, img->format );
    
    if ( status == STATUS_OK )
    {
        switch ( img->format ) 
        {
            case IMG_GRAY :
            {
                UInt8 gray;
                Color_GetGray ( color, &gray );
                memset ( img->planes[0].data, gray, img->height * img->planes[0].stride );
                break;
            }
            
            case IMG_RGB :
            {
                UInt8 red, green, blue;
                Color_GetRGB ( color, &red, &green, &blue );
                memset ( img->planes[0].data, red, img->height * img->planes[0].stride );
                memset ( img->planes[1].data, green, img->height * img->planes[1].stride );
                memset ( img->planes[2].data, blue, img->height * img->planes[2].stride );
                break;
            }

            case IMG_YUV420 :
            {
                UInt8 Y, U, V;
                Color_GetYUV ( color, &Y, &U, &V );

                memset ( img->planes[0].data, Y, img->height * img->planes[0].stride );
                memset ( img->planes[1].data, U,  ( img->height / 2 ) * img->planes[1].stride );
                memset ( img->planes[2].data, V, ( img->height / 2 ) * img->planes[2].stride );
                break;
            }
            
            case IMG_YUV444 :
            {
                UInt8 Y, U, V;
                Color_GetYUV ( color, &Y, &U, &V );

                memset ( img->planes[0].data, Y, img->height * img->planes[0].stride );
                memset ( img->planes[1].data, U, img->height * img->planes[1].stride );
                memset ( img->planes[2].data, V, img->height * img->planes[2].stride );
                break;
            }
        }
    }

    return status;
}


/*------------------------------------------------------------------------------
 * Image_Check
 *----------------------------------------------------------------------------*/
Int32 Image_Check (
    IN const Image* img,
    IN UInt32 requiredWidth,
    IN UInt32 requiredHeight,
    IN UInt8 requiredFormat)
{
    Int32 status = STATUS_OK;

    if ( img == NULL ) 
    {
        printf ( "Image_Check: Unnalocated image\n" );
        status = STATUS_FAIL;
    } 

    if ( status == STATUS_OK ) 
    {
        if ( img->format != requiredFormat )
        {
            printf ( "Image_Check: Not the required format\n" );
            status = STATUS_FAIL;
        }
    }

    if ( status == STATUS_OK ) 
    {
        if ( ( img->width != requiredWidth ) || ( img->height != requiredHeight ) )
        {
            printf ( "Image_Check: Not the required dimensions\n" );
            status = STATUS_FAIL;
        }
    }

    if ( status == STATUS_OK ) 
    {
        Int32 plane;
        UInt8 planes = 0;
        Image_GetNumPlanes ( img );
        for ( plane = 0; plane < planes && status == STATUS_OK; ++plane ) 
        {
            if ( img->planes[plane].data == NULL )
            {
                printf ( "Image_Check: Unallocated image plane\n" );
                status = STATUS_FAIL;
            }
        }
    }

    return status;
}

/*------------------------------------------------------------------------------
 * Image_GetImageSize
 *----------------------------------------------------------------------------*/
Int32 Image_GetImageSize(
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
        Image_GetNumPlanes ( img );
        size = 0;
        for ( plane = 0; plane < planes; ++plane ) 
        {
            Image_GetPlaneSize ( img->format, img->width, img->height, plane + 1, &planeSize );
            size += planeSize;
        }
    }
    return status;
}

/*------------------------------------------------------------------------------
 * Image_GetPlaneSize
 *----------------------------------------------------------------------------*/

Int32 Image_GetPlaneSize(
    IN UInt8 format,
    IN UInt32 width,
    IN UInt32 height,
    IN UInt8 plane,
    OUT UInt32* planeSize)
{
    Int32 status;
    
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
            case IMG_YUV420 :
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
            case IMG_YUV444 :
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
 * CreateImage
 *----------------------------------------------------------------------------*/
Int32 Image_Create(
	IN IMemory* heap,
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img)
{
    Int32 status;
    Image* newImg = NULL;
    UInt8 nrPlanes = 0;

    if ( img == NULL && heap != NULL ) 
    {
        printf ( "CreateImage: invalid arguments!\n" );
        status = STATUS_INVALID_ARGUMENT;
    } 
    else
    {
        UInt32 size = (UInt32)sizeof(Image);
        newImg = heap->vft->Alloc ( heap, size );

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
			newImg->heap = heap;

            status = STATUS_OK;
        }
    }

    if ( status == STATUS_OK )
    {
        nrPlanes = Image_GetNumPlanes ( newImg );
		if ( nrPlanes == STATUS_INVALID_ARGUMENT )
			status = STATUS_FAIL;
    }

    if ( status == STATUS_OK ) 
    {
        UInt8 plane;
        UInt32 planeSize = 0;

        for ( plane = 0; ((plane < nrPlanes) && (status == STATUS_OK)); ++plane ) 
        {
            status = Image_GetPlaneSize(format, width, height, plane + 1, &planeSize);

            if (status == STATUS_OK)
            {
                newImg->planes[plane].data = heap->vft->Alloc ( heap, planeSize );

                if (newImg->planes[plane].data == NULL)
                {
                    printf("CreateImage: could not allocate plane\n");
                    status = STATUS_FAIL;
                }
                if ( format == IMG_YUV420 && plane > 0 )
                {
                    newImg->planes[plane].stride = width / 2;
                }
                else
                {
                    newImg->planes[plane].stride = width;
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
 * Image_Destroy
 *----------------------------------------------------------------------------*/
void Image_Destroy (
    IN OUT Image** img ) 
{
    if (img != NULL && *img != NULL ) 
    {
        Int32 plane;

        for (plane = 0; plane < MAX_NUM_PLANES; ++plane)
        {
            if ((*img)->planes[plane].data != NULL)
            {
                //printf ( "DestroyImage: free plane %d\n", plane );
                //free((*img)->planes[plane].data);
				(*img)->heap->vft->Free ( (*img)->heap, (*img)->planes[plane].data );
            } else
            {
                printf ( "DestroyImage: empty image plane %d\n", plane );
            }
            
        }

       // printf ( "DestroyImage: free image\n" );

        //free(*img);

        *img = NULL;
    }
}

/*------------------------------------------------------------------------------
 * CropImage
 *----------------------------------------------------------------------------*/
Int32 Image_Crop (
    IN const Image* img,
    IN UInt32 cropWidth,
    IN UInt32 cropHeight,
    IN UInt32 offsetX,
    IN UInt32 offsetY, 
    OUT Image* crop )
{

    Int32 status = STATUS_OK;
    status = Image_Check ( img, img->width, img->height, img->format );
    if ( status == STATUS_OK )
    {
        status = Image_Check ( crop, crop->width, crop->height, crop->format );
    }

    if ( status == STATUS_OK ) 
    {
        crop->height = cropHeight;
        crop->width = cropWidth;
        crop->format = img -> format;
        crop->bpp = img -> bpp;
        crop->planes[0].stride = img -> planes[0].stride;
        crop->planes[0].data = ( UInt8* ) img -> planes[0].data + offsetY * img -> planes[0].stride + offsetX;
    }
    return status;
}

/*------------------------------------------------------------------------------
 * WriteImageToData
 *----------------------------------------------------------------------------*/

static Int32 WriteImgPlane ( 
    IN const ImgPlane* imgPlane,
    IN UInt32 imgWidth, 
    IN UInt32 imgHeight, 
    OUT FILE* file )
{
    UInt8* data;
    Int32 y, status, chunksWritten;

    assert ( imgPlane != NULL );
    assert ( file != NULL );                                                                                                                                                                                                                                                

    data = ( UInt8* ) imgPlane->data;

    if ( data == NULL )
    {
        printf ( "WriteImgPlane: Empty image plane\n" );
        status = STATUS_FAIL;
    } 
    else
    {
        status = STATUS_OK;
        for ( y = 0; y < imgHeight && status == STATUS_OK; ++y ) 
        {
            chunksWritten = fwrite ( data, imgWidth, 1, file );
            if ( chunksWritten != 1 )
            {
                status = STATUS_FAIL;
                printf ( "WriteImgPlane: Couldn't write data %d bytes\n", imgWidth );
            }
            data += imgPlane->stride;
        }
    }
    
    return status;
}

Int32 Image_WriteImageToFile ( 
    IN const Image* img,
    OUT FILE* file )
{
    Int32 status = STATUS_OK;

    if ( img == NULL || file == NULL ) 
    {
        printf ( "WriteImageToFile: Invalid input argument\n" );
        status = STATUS_FAIL;
    }

    if ( status == STATUS_OK )
    {
        status = Image_Check ( img, img->width, img->height, img->format );
    }

    if ( status == STATUS_OK )
    {
        switch ( img->format )
        {
            case IMG_GRAY :
            {
                status = WriteImgPlane ( &img->planes[0], img->width, img->height, file );
                break;
            }
            case IMG_RGB :
            {
                status = WriteImgPlane ( &img->planes[0], img->width, img->height, file );
                if ( status == STATUS_OK )
                {
                    status = WriteImgPlane ( &img->planes[1], img->width, img->height, file );
                }
                if ( status == STATUS_OK )
                {
                    status = WriteImgPlane ( &img->planes[2], img->width, img->height, file );
                }

                break;
            }
            case IMG_YUV420 :
            {
                status = WriteImgPlane ( &img->planes[0], img->width, img->height, file );
                if ( status == STATUS_OK )
                {
                    status = WriteImgPlane ( &img->planes[1], img->width / 2, img->height / 2, file );
                }
                if ( status == STATUS_OK )
                {
                    status = WriteImgPlane ( &img->planes[2], img->width / 2, img->height / 2, file );
                }
                break;
            }

            case IMG_YUV444 :
            {
                status = WriteImgPlane ( &img->planes[0], img->width, img->height, file );
                if ( status == STATUS_OK )
                {
                    status = WriteImgPlane ( &img->planes[1], img->width, img->height, file );
                }
                if ( status == STATUS_OK )
                {
                    status = WriteImgPlane ( &img->planes[2], img->width, img->height, file );
                }

                break;
            }
            default :
            {
                status = STATUS_FAIL;
                printf ( "WriteImageToFile: invalid image format\n" );
            }
        }
    }

    return status;
} 
