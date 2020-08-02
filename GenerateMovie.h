#ifndef GENERATE_MOVIE_H_INCLUDED
#define GENERATE_MOVIE_H_INCLUDED

#include "Macros.h"
#include "Image.h"

Int32 dirY[] = { -1, -1, 1, 1 };
Int32 dirX[] = { -1, 1, 1, -1 };

typedef struct Rectangle
{
    int top, left, width, heigth;
} Rectangle;

Int32 GenerateMovie (
    IN Int32 width,
    IN Int32 height,
    IN Int32 numFrames,
    IN const char* path );

#endif /* GENERATE_MOVIE_H_INCLUDED */