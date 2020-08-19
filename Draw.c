#include <stdio.h>
#include "Draw.h"

static void DrawRectangleGray ( Image* img, const Rectangle* rectangle, UInt32 color );
static void DrawRectangleRGB ( Image* img, const Rectangle* rectangle, UInt32 color );
static void DrawRectangleYUV ( Image* img, const Rectangle* rectangle, UInt32 color );

Int32 DrawRectangle ( 
    IN Image* img,
    IN Rectangle* rectangle,
    IN UInt32 color /* ( 0xRRGGBB ) */ )
{
    Int32 status;

    if ( img == NULL )
    {
        status = STATUS_FAIL;
    } 
    else
    {
        status = CheckImage ( img, img->width, img->height, img->format );
    }

    if ( status == STATUS_OK )
    {
        switch ( img->format ) 
        {
            case IMG_GRAY :
                DrawRectangleGray ( img, rectangle, color );
                break;
            case IMG_RGB : 
                DrawRectangleRGB ( img, rectangle, color );
                break;
            case IMG_YUV : 
                DrawRectangleYUV ( img, rectangle, color );
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

    assert ( img != NULL );
    assert ( img->format == IMG_GRAY );
    
    GetGray ( color, &gray );

    imgDataStride = img->planes[0].stride;
    imgData = ( UInt8* ) img->planes[0].data;

    DrawRect ( imgData, img->width, img->height, imgDataStride, gray, rectangle );
}

static void DrawRectangleRGB ( 
    IN OUT Image* img, 
    IN const Rectangle* rectangle, 
    IN UInt32 color ) 
{
    UInt8 red, green, blue;
    UInt32 imgDataStrideRed, imgDataStrideGreen, imgDataStrideBlue;
    UInt8* imgDataRed, *imgDataGreen, *imgDataBlue;

    assert ( img != NULL );
    assert ( img->format == IMG_RGB );
    
    GetRGB ( color, &red, &green, &blue );

    imgDataStrideRed = img->planes[0].stride;
    imgDataStrideGreen = img->planes[1].stride;
    imgDataStrideBlue = img->planes[2].stride;
    imgDataRed = ( UInt8* ) img->planes[0].data;
    imgDataGreen = ( UInt8* ) img->planes[1].data;
    imgDataBlue = ( UInt8* ) img->planes[2].data;

    DrawRect ( imgDataRed, img->width, img->height, imgDataStrideRed, red, rectangle );
    DrawRect ( imgDataGreen, img->width, img->height, imgDataStrideGreen, green, rectangle );
    DrawRect ( imgDataBlue, img->width, img->height, imgDataStrideBlue, blue, rectangle );
}

static void DrawRectangleYUV ( 
    IN OUT Image* img, 
    IN const Rectangle* rectangle, 
    IN UInt32 color )
{
    UInt8 Y, U, V;
    UInt32 imgDataStrideY, imgDataStrideU, imgDataStrideV;
    UInt8* imgDataY, *imgDataU, *imgDataV;
    Rectangle newRectangle;

    assert ( img != NULL );
    assert ( rectangle != NULL );
    assert ( img->format == IMG_YUV );
    
    GetYUV ( color, &Y, &U, &V );

    imgDataStrideY = img->planes[0].stride;
    imgDataStrideU= img->planes[1].stride;
    imgDataStrideV = img->planes[2].stride;
    imgDataY = ( UInt8* ) img->planes[0].data;
    imgDataU = ( UInt8* ) img->planes[1].data;
    imgDataV = ( UInt8* ) img->planes[2].data;

    DrawRect ( imgDataY, img->width , img->height, imgDataStrideY, Y, rectangle );

    newRectangle = *rectangle;
    newRectangle.top /= 2;
    newRectangle.left /= 2;
    newRectangle.width /= 2;
    newRectangle.height /= 2;
    DrawRect ( imgDataU, img->width / 2, img->height / 2, imgDataStrideU, U, &newRectangle );
    DrawRect ( imgDataV, img->width / 2, img->height / 2, imgDataStrideV, V, &newRectangle );
}
