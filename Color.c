#include <stdio.h>
#include <assert.h>
#include "Color.h"

void GetGray ( 
    IN UInt32 color, 
    OUT UInt8* gray )
{   
    UInt8 red, green, blue;

    assert ( gray != NULL );

    GetRGB ( color, &red, &green, &blue );

    *gray = ( red + green + blue ) / 3;
}

void GetRGB ( 
    IN UInt32 color, 
    OUT UInt8* red, 
    OUT UInt8* green, 
    OUT UInt8* blue )
{
    assert ( red != NULL );
    assert ( green != NULL );
    assert ( blue != NULL );

    *red = ( UInt8 ) ( color >> 16 );
    *green = ( UInt8 ) ( color >> 8 );
    *blue = ( UInt8 ) color;
}

void GetYUV ( 
    IN UInt32 color, 
    OUT UInt8* Y, 
    OUT UInt8* U, 
    OUT UInt8* V )
{
    UInt8 red, green, blue;

    GetRGB ( color, &red, &green, &blue );
    RGBToYUV ( red, green, blue, Y, U, V );
}

void RGBToYUV ( 
    IN UInt8 R,
    IN UInt8 G,
    IN UInt8 B,
    OUT UInt8* Y,
    OUT UInt8* U,
    OUT UInt8* V )
{
    assert ( Y != NULL );
    assert ( U != NULL );
    assert ( V != NULL );

    *Y = ( UInt8 ) ( 0.257f * R + 0.504f * G + 0.098f * B +  16 );
    *U = ( UInt8 ) ( -0.148f * R - 0.291f * G + 0.439f * B + 128 );
    *V = ( UInt8 ) ( 0.439f * R - 0.368f * G - 0.071f * B + 128 );
}

void YUVToRGB ( 
    IN UInt8 Y,
    IN UInt8 U,
    IN UInt8 V,
    OUT UInt8* R,
    OUT UInt8* G,
    OUT UInt8* B )
{
    assert ( R != NULL );
    assert ( G != NULL );
    assert ( B != NULL );

    *R = ( UInt8 ) ( 1.164f * ( Y - 16 ) + 1.596f * ( V - 128 ) );
    *G = ( UInt8 ) ( 1.164f * ( Y - 16 ) - 0.813f * ( V - 128 ) - 0.391f * ( U - 128 ) );
    *B = ( UInt8 ) ( 1.164f * ( Y - 16 ) + 2.018f * ( U - 128 ) );
}