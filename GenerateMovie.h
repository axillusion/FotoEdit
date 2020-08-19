#ifndef GENERATE_MOVIE_H_INCLUDED
#define GENERATE_MOVIE_H_INCLUDED

#include "Macros.h"
#include "Image.h"

Int32 GenerateMovie (
    IN Int32 width,
    IN Int32 height,
    IN Int32 numFrames,
    IN const char* path );

Int32 GetFormatFromPath ( 
    IN const char* path,
    OUT UInt8* format );

#endif /* GENERATE_MOVIE_H_INCLUDED */