#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "LibImage.h"
#include "ImagePriv.h"

static void CopyPlane ( 
    IN UInt32 width,
    IN UInt32 height,
    IN const ImgPlane* src,
    OUT ImgPlane* dst );

static Int32 ConvertImageGrayToGray ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageGrayToRGB ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageGrayToYUV420 ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageGrayToYUV444 ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageRGBToGray ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageRGBToRGB ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageRGBToYUV420 ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageRGBToYUV444 ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUV420ToGray ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUV420ToRGB ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUV420ToYUV420 ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUV420ToYUV444 ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUV444ToGray ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUV444ToRGB ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUV444ToYUV420 ( 
    IN const Image* src,
    OUT Image* dst );

static Int32 ConvertImageYUV444ToYUV444 ( 
    IN const Image* src,
    OUT Image* dst );


Int32 Image_Convert ( 
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
        status = Image_Check ( src, src->width, src->height, src->format );
    }

    if ( status == STATUS_OK ) 
    {
        status = Image_Check ( dst, src->width, src->height, dst->format );
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
            else if ( dst->format == IMG_YUV420 ) 
            {
                status = ConvertImageGrayToYUV420 ( src, dst );
            } 
            else if ( dst->format == IMG_YUV444 )
            {
                status = ConvertImageGrayToYUV444 ( src, dst );
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
            else if ( dst->format == IMG_YUV420 ) 
            {
                status = ConvertImageRGBToYUV420 ( src, dst );
            } 
            else if ( dst->format == IMG_YUV444 )
            {
                status = ConvertImageRGBToYUV444 ( src, dst );
            }
            else 
            {
                printf ( "ConvertImage: Invalid destination format\n" );
                status = STATUS_FAIL;
            }
        }
        else if ( src->format == IMG_YUV420 )
        {
            if ( dst->format == IMG_GRAY )
            {
                status = ConvertImageYUV420ToGray ( src, dst );
            } 
            else if ( dst->format == IMG_RGB )
            {
                status = ConvertImageYUV420ToRGB ( src, dst );
            } 
            else if ( dst->format == IMG_YUV420 ) 
            {
                status = ConvertImageYUV420ToYUV420 ( src, dst );
            } 
            else if ( dst->format == IMG_YUV444 )
            {
                status = ConvertImageYUV420ToYUV444 ( src, dst );
            }
            else 
            {
                printf ( "ConvertImage: Invalid destination format\n" );
                status = STATUS_FAIL;
            }
        }
        else if ( src->format == IMG_YUV444 )
        {
            if ( dst->format == IMG_GRAY )
            {
                status = ConvertImageYUV444ToGray ( src, dst );
            } 
            else if ( dst->format == IMG_RGB )
            {
                status = ConvertImageYUV444ToRGB ( src, dst );
            } 
            else if ( dst->format == IMG_YUV420 ) 
            {
                status = ConvertImageYUV444ToYUV420 ( src, dst );
            } 
            else if ( dst->format == IMG_YUV444 )
            {
                status = ConvertImageYUV444ToYUV444 ( src, dst );
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
    Int32 status;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_GRAY );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_GRAY );
    }

    if ( status == STATUS_OK )
    {
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
    }

    return status;
}

static Int32 ConvertImageGrayToRGB (
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_GRAY );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_RGB );
    }

    if ( status == STATUS_OK )
    {
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[1] );
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[2] );
    }

    return status;
}

static Int32 ConvertImageGrayToYUV420 (
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, i, color;
    UInt8 red, green, blue, Y, U, V;
    UInt8 *data, *dstDataY, *dstDataU, *dstDataV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_GRAY );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_YUV420 );
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
            Color_GetYUV ( color, &Y, &U, &V );
            dstDataY[i] = Y;
            dstDataU[i / 2] = U;
            dstDataV[i / 2] = V;
        }
    }

    return status;
}

static Int32 ConvertImageGrayToYUV444 ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, i, color;
    UInt8 red, green, blue, Y, U, V;
    UInt8 *data, *dstDataY, *dstDataU, *dstDataV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_GRAY );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_YUV444 );
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
            Color_GetYUV ( color, &Y, &U, &V );
            dstDataY[i] = Y;
            dstDataU[i] = U;
            dstDataV[i] = V;
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

    status = Image_Check ( src, src->width, src->height, IMG_RGB );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_GRAY );
    }

    if ( status == STATUS_OK )
    {
        data = ( UInt8* ) dst->planes[0].data;
        dataRed = ( UInt8* ) src->planes[0].data;
        dataGreen = ( UInt8* ) src->planes[1].data;
        dataBlue = ( UInt8* ) src->planes[2].data;
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
    Int32 status;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_RGB );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_RGB );
    }

    if ( status == STATUS_OK )
    {
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
        CopyPlane ( dst->width, dst->height, &src->planes[1], &dst->planes[1] );
        CopyPlane ( dst->width, dst->height, &src->planes[2], &dst->planes[2] );
    }

    return status;
}

static Int32 ConvertImageRGBToYUV420 ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y, x;
    UInt8 red, green, blue, Y, U, V;
    UInt8 *srcDataRed, *srcDataGreen, *srcDataBlue, *dstDataY, *dstDataU, *dstDataV;
    UInt32 srcStrideR, srcStrideG, srcStrideB, dstStrideY, dstStrideU, dstStrideV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_RGB );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_YUV420 );
    }

    if ( status == STATUS_OK )
    {
        srcDataRed = ( UInt8* ) src->planes[0].data;
        srcDataGreen = ( UInt8* ) src->planes[1].data;
        srcDataBlue = ( UInt8* ) src->planes[2].data;
        dstDataY = ( UInt8* ) dst->planes[0].data;
        dstDataU = ( UInt8* ) dst->planes[1].data;
        dstDataV = ( UInt8* ) dst->planes[2].data;

        srcStrideR = src->planes[0].stride;
        srcStrideG = src->planes[1].stride;
        srcStrideB = src->planes[2].stride;
        dstStrideY = dst->planes[0].stride;
        dstStrideU = dst->planes[1].stride;
        dstStrideV = dst->planes[2].stride;
        
        for ( y = 0; y < dst->height; ++y )
        {
            for ( x = 0; x < dst->width; ++x )
            {
                red = srcDataRed[y * srcStrideR + x];
                green = srcDataGreen[y * srcStrideG + x];
                blue = srcDataBlue[y * srcStrideB + x];
                Color_RGBToYUV ( red, green, blue, &Y, &U, &V );
                dstDataY[y * dstStrideY + x] = Y;
                if ( ( y % 2 == 0 ) && ( x % 2 == 0 ) )
                {
                    dstDataU[( y / 2 ) * dstStrideU + x / 2] = U;
                    dstDataV[( y / 2 ) * dstStrideV + x / 2] = V;
                }
            }
        }
    }

    return status;
}

static Int32 ConvertImageRGBToYUV444 ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y, x;
    UInt8 red, green, blue, Y, U, V;
    UInt8 *srcDataRed, *srcDataGreen, *srcDataBlue, *dstDataY, *dstDataU, *dstDataV;
    UInt32 srcStrideR, srcStrideG, srcStrideB, dstStrideY, dstStrideU, dstStrideV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_RGB );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_YUV444 );
    }

    if ( status == STATUS_OK )
    {
        srcDataRed = ( UInt8* ) src->planes[0].data;
        srcDataGreen = ( UInt8* ) src->planes[1].data;
        srcDataBlue = ( UInt8* ) src->planes[2].data;
        dstDataY = ( UInt8* ) dst->planes[0].data;
        dstDataU = ( UInt8* ) dst->planes[1].data;
        dstDataV = ( UInt8* ) dst->planes[2].data;

        srcStrideR = src->planes[0].stride;
        srcStrideG = src->planes[1].stride;
        srcStrideB = src->planes[2].stride;
        dstStrideY = dst->planes[0].stride;
        dstStrideU = dst->planes[1].stride;
        dstStrideV = dst->planes[2].stride;
        
        for ( y = 0; y < dst->height; ++y )
        {
            for ( x = 0; x < dst->width; ++x )
            {
                red = srcDataRed[y * srcStrideR + x];
                green = srcDataGreen[y * srcStrideG + x];
                blue = srcDataBlue[y * srcStrideB + x];
                Color_RGBToYUV ( red, green, blue, &Y, &U, &V );
                dstDataY[y * dstStrideY + x] = Y;
                dstDataU[y * dstStrideU + x] = U;
                dstDataV[y * dstStrideV + x] = V;
            }
        }
    }

    return status;
}

static Int32 ConvertImageYUV420ToGray ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_YUV420 );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_GRAY );
    }

    if ( status == STATUS_OK )
    {
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
    }

    return status;
}

static Int32 ConvertImageYUV420ToRGB ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y, x;
    UInt32 srcStrideY, srcStrideU, srcStrideV, dstStrideR, dstStrideG, dstStrideB;
    UInt8 *srcDataY, *srcDataU, *srcDataV, *dstDataR, *dstDataG, *dstDataB; 
    UInt8 Y, U, V;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_YUV420 );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_RGB );
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
                U = srcDataU[( y / 2 ) * srcStrideU + x / 2];
                V = srcDataV[( y / 2 ) * srcStrideV + x / 2];
                
                Color_YUVToRGB ( Y, U, V, 
                    &dstDataR[y * dstStrideR + x], 
                    &dstDataG[y * dstStrideG + x], 
                    &dstDataB[y * dstStrideB + x] );
            }
        }
    }

    return status;
}

static Int32 ConvertImageYUV420ToYUV420 ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_YUV420 );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_YUV420 );
    }

    if ( status == STATUS_OK )
    {
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
        CopyPlane ( dst->width / 2, dst->height / 2, &src->planes[1], &dst->planes[1] );
        CopyPlane ( dst->width / 2, dst->height / 2, &src->planes[2], &dst->planes[2] );

    }

    return status;
}

static Int32 ConvertImageYUV420ToYUV444 ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y, x;
    UInt8 *srcDataU, *srcDataV, *dstDataU, *dstDataV;
    UInt32 srcStrideU, srcStrideV, dstStrideU, dstStrideV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_YUV420 );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_YUV444 );
    }

    if ( status == STATUS_OK )
    {
        srcStrideU = src->planes[1].stride;
        srcStrideV = src->planes[2].stride;
        dstStrideU = dst->planes[1].stride;
        dstStrideV = dst->planes[2].stride;

        srcDataU = ( UInt8* ) src->planes[1].data;
        srcDataV = ( UInt8* ) src->planes[2].data;
        dstDataU = ( UInt8* ) dst->planes[1].data;
        dstDataV = ( UInt8* ) dst->planes[2].data;

        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
        
        for ( y = 0; y < dst->height; ++y )
        {
            for ( x = 0; x < dst->width; ++x )
            {
                dstDataU[y * dstStrideU + x] = srcDataU[( y / 2 ) * srcStrideU + x / 2];
                dstDataV[y * dstStrideV + x] = srcDataV[( y / 2 ) * srcStrideV + x / 2];
            }
        }
    }

    return status;
}

static Int32 ConvertImageYUV444ToGray ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_YUV444 );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_GRAY );
    }

    if ( status == STATUS_OK )
    {
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
    }

    return status;
}

static Int32 ConvertImageYUV444ToRGB ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y, x;
    UInt32 srcStrideY, srcStrideU, srcStrideV, dstStrideR, dstStrideG, dstStrideB;
    UInt8 *srcDataY, *srcDataU, *srcDataV, *dstDataR, *dstDataG, *dstDataB; 
    UInt8 Y, U, V;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_YUV444 );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_RGB );
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
                V = srcDataV[y * srcStrideV + x];
                
                Color_YUVToRGB ( Y, U, V, 
                    &dstDataR[y * dstStrideR + x], 
                    &dstDataG[y * dstStrideG + x], 
                    &dstDataB[y * dstStrideB + x] );
            }
        }
    }

    return status;
}

static Int32 ConvertImageYUV444ToYUV420 ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status, y, x;
    UInt8 *srcDataU, *srcDataV, *dstDataU, *dstDataV;
    UInt32 srcStrideU, srcStrideV, dstStrideU, dstStrideV;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_YUV444 );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_YUV420 );
    }

    if ( status == STATUS_OK )
    {
        srcStrideU = src->planes[1].stride;
        srcStrideV = src->planes[2].stride;
        dstStrideU = dst->planes[1].stride;
        dstStrideV = dst->planes[2].stride;

        srcDataU = ( UInt8* ) src->planes[1].data;
        srcDataV = ( UInt8* ) src->planes[2].data;
        dstDataU = ( UInt8* ) dst->planes[1].data;
        dstDataV = ( UInt8* ) dst->planes[2].data;

        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
        
        for ( y = 0; y < dst->height / 2; ++y )
        {
            for ( x = 0; x < dst->width / 2; ++x )
            {
                dstDataU[y * dstStrideU + x] = srcDataU[2 * y * srcStrideU + x * 2];
                dstDataV[y * dstStrideV + x] = srcDataV[2 * y * srcStrideV + x * 2];
            }
        }
    }

    return status;
}

static Int32 ConvertImageYUV444ToYUV444 ( 
    IN const Image* src,
    OUT Image* dst )
{
    Int32 status;

    assert ( src != NULL );
    assert ( dst != NULL );

    status = Image_Check ( src, src->width, src->height, IMG_YUV444 );

    if ( status == STATUS_OK )
    {
        status = Image_Check ( dst, src->width, src->height, IMG_YUV444);
    }

    if ( status == STATUS_OK )
    {
        CopyPlane ( dst->width, dst->height, &src->planes[0], &dst->planes[0] );
        CopyPlane ( dst->width, dst->height, &src->planes[1], &dst->planes[1] );
        CopyPlane ( dst->width, dst->height, &src->planes[2], &dst->planes[2] );
    }

    return status;
}

static void CopyPlane ( 
    IN UInt32 width,
    IN UInt32 height,
    IN const ImgPlane* src,
    OUT ImgPlane* dst )
{
    Int32 y; 

    assert ( src != NULL );
    assert ( dst != NULL );
    assert ( src->data != NULL );
    assert ( dst->data != NULL );
    
    for ( y = 0; y < height; ++y )
    {
        memcpy ( ( UInt8* ) dst->data + dst->stride * y, ( UInt8* ) src->data + src->stride * y, width );
    }
}