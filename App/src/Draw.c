#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "LibImage.h"
#include "GenerateMovie.h"
#include "Draw.h"

static void DrawRectangleGray ( Image* img, const Rectangle* rectangle, UInt32 color );
static void DrawRectangleRGB ( Image* img, const Rectangle* rectangle, UInt32 color );
static void DrawRectangleYUV420 ( Image* img, const Rectangle* rectangle, UInt32 color );
static void DrawRectangleYUV444 ( Image* img, const Rectangle* rectangle, UInt32 color );

Int32 DrawRectangle ( 
    IN Image* img,
    IN Rectangle* rectangle,
    IN UInt32 color /* ( 0xRRGGBB ) */ )
{
    Int32 status;
	UInt32 imgWidth, imgHeight, imgFormat;
	imgWidth = Image_GetWidth ( img );
	imgHeight = Image_GetHeight ( img );
	imgFormat = Image_GetFormat ( img );

    if ( img == NULL )
    {
        status = STATUS_FAIL;
    } 
    else
    {
        status = Image_Check ( img, imgWidth, imgHeight, imgFormat );
    }

    if ( status == STATUS_OK )
    {
        switch ( imgFormat ) 
        {
            case IMG_GRAY :
                DrawRectangleGray ( img, rectangle, color );
                break;
            case IMG_RGB : 
                DrawRectangleRGB ( img, rectangle, color );
                break;
            case IMG_YUV420 : 
                DrawRectangleYUV420 ( img, rectangle, color );
                break;
            case IMG_YUV444 :
                DrawRectangleYUV444 ( img, rectangle, color );
                break;
            default :
                printf ( "DrawRectangle: Invalid image format\n" );
                status = STATUS_FAIL;
                break;
        }
    }

    return status;
}

static void ClampRectangle ( const Rectangle* origRectangle, Int32 top, Int32 left, Int32 width, Int32 height, Rectangle* clampedRectangle )
{

    assert ( origRectangle != NULL );
    assert ( clampedRectangle != NULL );

    *clampedRectangle = *origRectangle;

    if ( clampedRectangle->top < top ) 
    {
        clampedRectangle->height = clampedRectangle->height + clampedRectangle->top;
        clampedRectangle->top = 0;
    }

    if ( clampedRectangle->top + clampedRectangle->height > height ) 
    {
        clampedRectangle->height = height - clampedRectangle->top;
    }

    if ( clampedRectangle->left < left )
    {
        clampedRectangle->width = clampedRectangle->width + clampedRectangle->left;
        clampedRectangle->left = 0;
    }

    if ( clampedRectangle->left + clampedRectangle->width > width )
    {
        clampedRectangle->width = width - clampedRectangle->left;
    }
}

static void DrawRect ( 
    IN UInt8* data,
    IN UInt32 dataWidth,
    IN UInt32 dataHeight,
    IN UInt32 dataStride,
    IN UInt8 color, 
    IN const Rectangle* rectangle )
{
    Rectangle clampedRectangle;
    Int32 top, left, width, height, y;

    assert ( data != NULL );
    assert ( rectangle != NULL );

    ClampRectangle ( rectangle, 0, 0, dataWidth, dataHeight, &clampedRectangle );

    top = clampedRectangle.top;
    left = clampedRectangle.left;
    width = clampedRectangle.width;
    height = clampedRectangle.height;

    memset ( data + top * dataStride + left, color, width );
    memset ( data + ( top + height - 1 ) * dataStride + left, color, width );
    for ( y = top + 1; y < top + height - 1; ++y ) 
    {
        data[y * dataStride + left] = color;
        data[y * dataStride + left + width - 1] = color;
    }
}

static void DrawRectangleGray ( 
    IN OUT Image* img,
    IN const Rectangle* rectangle, 
    IN UInt32 color )
{
    UInt8 gray;
    UInt32 imgDataStride;
    UInt8* imgData;

	UInt32 imgWidth, imgHeight, imgFormat;
	imgWidth = Image_GetWidth ( img );
	imgHeight = Image_GetHeight ( img );
	imgFormat = Image_GetFormat ( img );

    assert ( img != NULL );
    assert ( imgFormat == IMG_GRAY );
    
    Color_GetGray ( color, &gray );

    imgDataStride = Image_GetPlaneStride ( img, 0 );
    imgData = ( UInt8* ) Image_GetPlaneData ( img, 0 );

    DrawRect ( imgData, imgWidth, imgHeight, imgDataStride, gray, rectangle );
}

static void DrawRectangleRGB ( 
    IN OUT Image* img, 
    IN const Rectangle* rectangle, 
    IN UInt32 color ) 
{
    UInt8 red, green, blue;
    UInt32 imgDataStrideRed, imgDataStrideGreen, imgDataStrideBlue;
    UInt8* imgDataRed, *imgDataGreen, *imgDataBlue;
	UInt32 imgWidth, imgHeight, imgFormat;
	imgWidth = Image_GetWidth ( img );
	imgHeight = Image_GetHeight ( img );
	imgFormat = Image_GetFormat ( img );

    assert ( img != NULL );
    assert ( imgFormat == IMG_RGB );
    
    Color_GetRGB ( color, &red, &green, &blue );

    imgDataStrideRed = Image_GetPlaneStride ( img, 0 );
    imgDataStrideGreen= Image_GetPlaneStride ( img, 1 );
    imgDataStrideBlue = Image_GetPlaneStride ( img, 2 );
    imgDataRed = ( UInt8* ) Image_GetPlaneData ( img, 0 );
    imgDataGreen = ( UInt8* ) Image_GetPlaneData ( img, 1 );
    imgDataBlue = ( UInt8* ) Image_GetPlaneData ( img, 2 );


    DrawRect ( imgDataRed, imgWidth, imgHeight, imgDataStrideRed, red, rectangle );
    DrawRect ( imgDataGreen, imgWidth, imgHeight, imgDataStrideGreen, green, rectangle );
    DrawRect ( imgDataBlue, imgWidth, imgHeight, imgDataStrideBlue, blue, rectangle );
}

static void DrawRectangleYUV420 ( 
    IN OUT Image* img, 
    IN const Rectangle* rectangle, 
    IN UInt32 color )
{
    UInt8 Y, U, V;
    UInt32 imgDataStrideY, imgDataStrideU, imgDataStrideV;
    UInt8* imgDataY, *imgDataU, *imgDataV;
    Rectangle newRectangle;
	UInt32 imgWidth, imgHeight, imgFormat;
	imgWidth = Image_GetWidth ( img );
	imgHeight = Image_GetHeight ( img );
	imgFormat = Image_GetFormat ( img );

    assert ( img != NULL );
    assert ( rectangle != NULL );
    assert ( imgFormat == IMG_YUV420 );
    
    Color_GetYUV ( color, &Y, &U, &V );

    imgDataStrideY = Image_GetPlaneStride ( img, 0 );
    imgDataStrideU= Image_GetPlaneStride ( img, 1 );
    imgDataStrideV = Image_GetPlaneStride ( img, 2 );
    imgDataY = ( UInt8* ) Image_GetPlaneData ( img, 0 );
    imgDataU = ( UInt8* ) Image_GetPlaneData ( img, 1 );
    imgDataV = ( UInt8* ) Image_GetPlaneData ( img, 2 );

    DrawRect ( imgDataY, imgWidth , imgHeight, imgDataStrideY, Y, rectangle );

    newRectangle = *rectangle;
    newRectangle.top /= 2;
    newRectangle.left /= 2;
    newRectangle.width /= 2;
    newRectangle.height /= 2;
    DrawRect ( imgDataU, imgWidth / 2, imgHeight / 2, imgDataStrideU, U, &newRectangle );
    DrawRect ( imgDataV, imgWidth / 2, imgHeight / 2, imgDataStrideV, V, &newRectangle );
}

static void DrawRectangleYUV444 ( 
    IN OUT Image* img, 
    IN const Rectangle* rectangle, 
    IN UInt32 color )
{
    UInt8 Y, U, V;
    UInt32 imgDataStrideY, imgDataStrideU, imgDataStrideV;
    UInt8* imgDataY, *imgDataU, *imgDataV;
	UInt32 imgWidth, imgHeight, imgFormat;
	imgWidth = Image_GetWidth ( img );
	imgHeight = Image_GetHeight ( img );
	imgFormat = Image_GetFormat ( img );

    assert ( img != NULL );
    assert ( rectangle != NULL );
    assert ( imgFormat == IMG_YUV444 );
    
    Color_GetYUV ( color, &Y, &U, &V );

    imgDataStrideY = Image_GetPlaneStride ( img, 0 );
    imgDataStrideU= Image_GetPlaneStride ( img, 1 );
    imgDataStrideV = Image_GetPlaneStride ( img, 2 );
    imgDataY = ( UInt8* ) Image_GetPlaneData ( img, 0 );
    imgDataU = ( UInt8* ) Image_GetPlaneData ( img, 1 );
    imgDataV = ( UInt8* ) Image_GetPlaneData ( img, 2 );

    DrawRect ( imgDataY, imgWidth , imgHeight, imgDataStrideY, Y, rectangle );
    DrawRect ( imgDataU, imgWidth, imgHeight, imgDataStrideU, U, rectangle );
    DrawRect ( imgDataV, imgWidth, imgHeight, imgDataStrideV, V, rectangle );
}
