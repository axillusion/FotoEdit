#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "Macros.h"
#include "Image.h"

Int32 DrawRectangle ( 
    IN Image* img,
    IN Rectangle* rectangle,
    IN UInt32 color /* ( 0xRRGGBB ) */ );

#endif // DRAW_H_INCLUDED