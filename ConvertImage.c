#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Macros.h"
#include "Image.h"
#include "Color.h"

static Int32 ConvertImageGrayToGray ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageGrayToRGB ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageGrayToYUV ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageRGBToGray ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageRGBToRGB ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageRGBToYUV ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUVToGray ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUVToRGB ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUVToYUV ( 
    IN const Image* src,
    OUT Image* dst );

Int32 ConvertImage ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status = STATUS_OK;

    if ( src == NULL || dst == NULL )
    {
        printf ( "ConvertImage: Invalid pointers given\n" );
        status = STATUS_FAIL;
    }
    
    if ( status == STATUS_OK ) 
    {
        status = CheckImage ( src, src->width, src->height, src->format );
    }

    if ( status == STATUS_OK ) 
    {
        status = CheckImage ( dst, src->width, src->height, dst->format );
    }

    if ( status == STATUS_OK )
    {
        if ( src->format == IMG_GRAY )
        {
            if ( dst->format == IMG_GRAY )
            {
                status = ConvertImageGrayToGray ( src, dst );
            } 
            else if ( dst->format == IMG_RGB )
            {
                status = ConvertImageGrayToRGB ( src, dst );
            } 
            else if ( dst->format == IMG_YUV ) 
            {
                status = ConvertImageGrayToYUV ( src, dst );
            } 
            else 
            {
                printf ( "ConvertImage: Invalid destination format\n" );
                status = STATUS_FAIL;
            }
        }
        else if ( src->format == IMG_RGB )
        {
            if ( dst->format == IMG_GRAY )
            {
                status = ConvertImageRGBToGray ( src, dst );
            } 
            else if ( dst->format == IMG_RGB )
            {
                status = ConvertImageRGBToRGB ( src, dst );
            } 
            else if ( dst->format == IMG_YUV ) 
            {
                status = ConvertImageRGBToYUV ( src, dst );
            } 
            else 
            {
                printf ( "ConvertImage: Invalid destination format\n" );
                status = STATUS_FAIL;
            }
        }
        else if ( src->format == IMG_YUV )
        {
            if ( dst->format == IMG_GRAY )
            {
                status = ConvertImageYUVToGray ( src, dst );
            } 
            else if ( dst->format == IMG_RGB )
            {
                status = ConvertImageYUVToRGB ( src, dst );
            } 
            else if ( dst->format == IMG_YUV ) 
            {
                status = ConvertImageYUVToYUV ( src, dst );
            } 
            else 
            {
                printf ( "ConvertImage: Invalid destination format\n" );
                status = STATUS_FAIL;
            }
        }
        else
        {
            printf ( "ConvertImage: Invalidn source format\n" );
            status = STATUS_FAIL;
        }
        
    }

    return status;
}

static Int32 ConvertImageGrayToGray ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y;
    UInt32 srcStride, dstStride;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_GRAY );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_GRAY );
    }

    if ( status == STATUS_OK )
    {
        srcStride = src->planes[0].stride;
        dstStride = dst->planes[0].stride;
        for ( y = 0; y < dst->height; ++y ) 
        {
            memcpy ( dst->planes[0].data + dstStride * y, src->planes[0].data + srcStride * y, dst->width );
        }
    }

    return status;
}

static Int32 ConvertImageGrayToRGB (
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, srcStride, dstStrideR, dstStrideG, dstStrideB, y;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_GRAY );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_RGB );
    }

    if ( status == STATUS_OK )
    {
        srcStride = src->planes[0].stride;
        dstStrideR = dst->planes[0].stride;
        dstStrideG = dst->planes[1].stride;
        dstStrideB = dst->planes[2].stride;
        for ( y = 0; y < dst->height; ++y ) 
        {
            memcpy ( dst->planes[0].data + dstStrideR * y, src->planes[0].data + srcStride * y, dst->width );
            memcpy ( dst->planes[1].data + dstStrideG * y, src->planes[0].data + srcStride * y, dst->width );
            memcpy ( dst->planes[2].data + dstStrideB * y, src->planes[0].data + srcStride * y, dst->width );
        }
    }

    return status;
}

static Int32 ConvertImageGrayToYUV (
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, i, color;
    UInt8 red, green, blue, Y, U, V;
    UInt8 *data, *dstDataY, *dstDataU, *dstDataV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_GRAY );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_YUV );
    }

    if ( status == STATUS_OK )
    {
        data = ( UInt8* ) src->planes[0].data;
        dstDataY = ( UInt8* ) dst->planes[0].data;
        dstDataU = ( UInt8* ) dst->planes[1].data;
        dstDataV = ( UInt8* ) dst->planes[2].data;

        for ( i = 0; i < src->planes[0].stride * src->height; ++i )
        {
            red = green = blue = data[i];
            color = blue + green * ( 1<<8 ) + red * ( 1<<16 );
            GetYUV ( color, &Y, &U, &V );
            dstDataY[i] = Y;
            dstDataU[i / 2] = U;
            dstDataV[i / 2] = V;
        }
    }

    return status;
}

static Int32 ConvertImageRGBToGray ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y, x;
    UInt8 *dataRed, *dataBlue, *dataGreen, *data;
    UInt32 dataStrideRed, dataStrideGreen, dataStrideBlue, dataStride;
    UInt8 red, green, blue;
    
    status = STATUS_OK;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_RGB );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_GRAY );
    }

    if ( status == STATUS_OK )
    {
        data = ( UInt8 ) dst->planes[0].data;
        dataRed = ( UInt8 ) src->planes[0].data;
        dataGreen = ( UInt8 ) src->planes[1].data;
        dataBlue = ( UInt8 ) src->planes[2].data;
        dataStride = dst->planes[0].stride;
        dataStrideRed = src->planes[0].stride;
        dataStrideGreen = src->planes[1].stride;
        dataStrideBlue = src->planes[2].stride;

        for ( y = 0; y < dst->height; ++y )
        {
            for ( x = 0; x < dst->width; ++x )
            {
                red = dataRed[y * dataStrideRed + x];
                green = dataGreen[y * dataStrideGreen + x];
                blue = dataBlue[y * dataStrideBlue + x];
                data[y * dataStride + x] = ( red + green + blue ) / 3;
            }
        }
    }

    return status;
}

static Int32 ConvertImageRGBToRGB ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y;
    UInt32 srcStrideRed, srcStrideGreen, srcStrideBlue, dstStrideRed, dstStrideGreen, dstStrideBlue;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_RGB );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_RGB );
    }

    if ( status == STATUS_OK )
    {
        srcStrideRed = src->planes[0].stride;
        srcStrideGreen = src->planes[1].stride;
        srcStrideBlue = src->planes[2].stride;
        dstStrideRed = dst->planes[0].stride;
        dstStrideGreen = dst->planes[1].stride;
        dstStrideBlue = dst->planes[2].stride;

        for ( y = 0; y < dst->height; ++y ) 
        {
            memcpy ( dst->planes[0].data + dstStrideRed * y, src->planes[0].data + srcStrideRed * y, dst->width );
            memcpy ( dst->planes[1].data + dstStrideGreen * y, src->planes[1].data + srcStrideGreen * y, dst->width );
            memcpy ( dst->planes[2].data + dstStrideBlue * y, src->planes[2].data + srcStrideBlue * y, dst->width );
        }
    }

    return status;
}

static Int32 ConvertImageRGBToYUV ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, i;
    UInt32 color;
    UInt8 red, green, blue, Y, U, V;
    UInt8 *srcDataRed, *srcDataGreen, *srcDataBlue, *dstDataY, *dstDataU, *dstDataV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_RGB );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_RGB );
    }

    if ( status == STATUS_OK )
    {
        srcDataRed = ( UInt8* ) src->planes[0].data;
        srcDataGreen = ( UInt8* ) src->planes[1].data;
        srcDataBlue = ( UInt8* ) src->planes[2].data;
        dstDataY = ( UInt8* ) dst->planes[0].data;
        dstDataU = ( UInt8* ) dst->planes[1].data;
        dstDataV = ( UInt8* ) dst->planes[2].data;

        // nu tine cont de stride-urile diferite

        for ( i = 0; i < src->planes[0].stride * src->height; ++i )
        {
            red = srcDataRed[i];
            green = srcDataGreen[i];
            blue = srcDataBlue[i];
            color = blue + green * ( 1<<8 ) + red * ( 1<<16 );
            GetYUV ( color, &Y, &U, &V );
            dstDataY[i] = Y;
            dstDataU[i / 2] = U;
            dstDataV[i / 2] = V;
        }
    }

    return status;
}

static Int32 ConvertImageYUVToGray ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y;
    UInt32 srcStride, dstStride;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_YUV );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_GRAY );
    }

    if ( status == STATUS_OK )
    {
        srcStride = src->planes[0].stride;
        dstStride = dst->planes[0].stride;
        for ( y = 0; y < dst->height; ++y ) 
        {
            memcpy ( dst->planes[0].data + dstStride * y, src->planes[0].data + srcStride * y, dst->width );
        }
    }

    return status;
}

static Int32 ConvertImageYUVToRGB ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y, x;
    UInt32 srcStrideY, srcStrideU, srcStrideV, dstStrideR, dstStrideG, dstStrideB;
    UInt8 *srcDataY, *srcDataU, *srcDataV, *dstDataR, *dstDataG, *dstDataB; 
    UInt8 Y, U, V;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_YUV );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_RGB );
    }

    if ( status == STATUS_OK )
    {
        srcStrideY = src->planes[0].stride;
        srcStrideU = src->planes[1].stride;
        srcStrideV = src->planes[2].stride;
        dstStrideR = dst->planes[0].stride;
        dstStrideG = dst->planes[1].stride;
        dstStrideB = dst->planes[2].stride;

        srcDataY = ( UInt8* ) src->planes[0].data;
        srcDataU = ( UInt8* ) src->planes[1].data;
        srcDataV = ( UInt8* ) src->planes[2].data;
        dstDataR = ( UInt8* ) dst->planes[0].data;
        dstDataG = ( UInt8* ) dst->planes[1].data;
        dstDataB = ( UInt8* ) dst->planes[2].data;

        for ( y = 0; y < dst->height; ++y ) 
        {
            for ( x = 0; x < dst->width; ++x )
            {
                Y = srcDataY[y * srcStrideY + x];
                U = srcDataU[y * srcStrideU + x];
                V = srcDataU[y * srcStrideV + x];
                dstDataR[y * dstStrideR + x] = ( UInt8 ) ( 1.164 * ( Y - 16 ) + 1.596 * ( V - 128 ) );
                dstDataG[y * dstStrideG + x] = ( UInt8 ) ( 1.164 * ( Y - 16 ) - 0.813 * ( V - 128 ) - 0.391 * ( U - 128 ) );
                dstDataB[y * dstStrideB + x] = ( UInt8 ) ( 1.164 * ( Y - 16 ) + 2.018 * ( U - 128 ) );
            }
        }
    }

    return status;
}

static Int32 ConvertImageYUVToYUV ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y;
    UInt32 srcStrideY, srcStrideU, srcStrideV, dstStrideY, dstStrideU, dstStrideV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = CheckImage ( src, src->width, src->height, IMG_YUV );

    if ( status == STATUS_OK )
    {
        status = CheckImage ( dst, src->width, src->height, IMG_YUV );
    }

    if ( status == STATUS_OK )
    {
        srcStrideY = src->planes[0].stride;
        srcStrideU = src->planes[1].stride;
        srcStrideV = src->planes[2].stride;
        dstStrideY = dst->planes[0].stride;
        dstStrideU = dst->planes[1].stride;
        dstStrideV = dst->planes[2].stride;

        for ( y = 0; y < dst->height; ++y ) 
        {
            memcpy ( dst->planes[0].data + dstStrideY * y, src->planes[0].data + srcStrideY * y, dst->width );
            memcpy ( dst->planes[1].data + dstStrideU * ( y / 2 ), src->planes[1].data + srcStrideU * ( y / 2 ), dst->width / 2 );
            memcpy ( dst->planes[2].data + dstStrideV * ( y / 2 ), src->planes[2].data + srcStrideV * ( y / 2 ), dst->width / 2 );
        }
    }

    return status;
}