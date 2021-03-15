#ifndef GENERATE_MOVIE_H_INCLUDED
#define GENERATE_MOVIE_H_INCLUDED

#include "Defines.h"

typedef struct Rectangle
{
    Int32 top, left, width, height;
} Rectangle;

Int32 GenerateMovie (
	IN IMemory* heap,
    IN Int32 width,
    IN Int32 height,
    IN Int32 numFrames,
    IN Int32 subsampling,
    IN const char* path );

Int32 GetFormatFromPath ( 
    IN const char* path,
    IN Int32 subsampling,
    OUT UInt8* format );

#endif /* GENERATE_MOVIE_H_INCLUDED */