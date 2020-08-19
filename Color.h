#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "Macros.h"

void GetRGB ( UInt32 color, UInt8* red, UInt8* green, UInt8* blue );
void GetYUV ( UInt32 color, UInt8* Y, UInt8* U, UInt8* V );
void GetGray ( UInt32 color, UInt8* gray );

void RGBToYUV ( 
    IN UInt8 R,
    IN UInt8 G,
    IN UInt8 B,
    OUT UInt8* Y,
    OUT UInt8* U,
    OUT UInt8* V );

void YUVToRGB ( 
    IN UInt8 Y,
    IN UInt8 U,
    IN UInt8 V,
    OUT UInt8* R,
    OUT UInt8* G,
    OUT UInt8* B );

#endif // COLOR_H_INCLUDED