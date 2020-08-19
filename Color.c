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

    assert ( Y != NULL );
    assert ( U != NULL );
    assert ( V != NULL );

    GetRGB ( color, &red, &green, &blue );

    *Y = ( UInt8 ) ( 0.257f * red + 0.504f * green + 0.098f * blue +  16 );
    *U = ( UInt8 ) ( -0.148f * red - 0.291f * green + 0.439f * blue + 128 );
    *V = ( UInt8 ) ( 0.439f * red - 0.368f * green - 0.071f * blue + 128 );
}
