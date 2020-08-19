#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "Macros.h"

void GetRGB ( UInt32 color, UInt8* red, UInt8* green, UInt8* blue );
void GetYUV ( UInt32 color, UInt8* Y, UInt8* U, UInt8* V );
void GetGray ( UInt32 color, UInt8* gray );

#endif // COLOR_H_INCLUDED