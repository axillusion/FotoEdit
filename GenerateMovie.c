#include <string.h>
#include <stdio.h>
#include "GenerateMovie.h"

#define TABLE_WIDTH 1024
#define TABLE_HEIGHT 1024

Rectangle rectangle;

/* C:/output/video1.bw */

Int32 GenerateMovie (
    IN Int32 width,
    IN Int32 height,
    IN Int32 numFrames,
    IN const char* path )
{
    Int32 status = STATUS_OK;
    Int32 format;
    Int32 i;
    FILE* fout = NULL;
    Image* img;

    if ( ( path == NULL ) || ( height == 0 ) || ( width == 0 ) || ( numFrames == 0 ) )
    {
        status = STATUS_FAIL;
        printf ( "GenerateMovie: Invalid input argument\n");
    }
    
    if ( status == STATUS_OK )
    {
        fout = fopen ( path, "wb" );
        if ( fout == NULL )
        {
            status = STATUS_FAIL;
            printf ( "GenerateMovie: Could not open file %s\n", path );
        }
    }

    if ( status == STATUS_OK ) 
    {
        Int32 lastDotPos = -1;

        i = 0;
        while ( path[i] != 0 ) 
        {
            if ( path[i] == '.' )
            {
                lastDotPos = i;
            }
            ++i;
        }
        
        if ( lastDotPos == -1 )
        {
            status = STATUS_FAIL;
            printf ( "GenerateMovie: Invalid path\n" );
        } 
        else
        {
            const char* ext = path + lastDotPos + 1;
            if ( strcmp ( ext, "bw" ) == 0 )
            {
                format = IMG_GRAY;
            } 
            else if ( strcmp ( ext, "rgb" ) == 0 )
            {
                format = IMG_RGB;
            } 
            else if ( strcmp ( ext, "yuv" ) == 0 ) 
            {
                format = IMG_YUV;
            } 
            else
            {
                status = STATUS_FAIL;
                printf ( "GenerateMovie: Invalid file format\n" );
            }
        }
    }

    if ( status == STATUS_OK )
    {
        status = CreateImage ( format, 8, width, height, &img ); 
    }

    if ( status == STATUS_OK ) 
    {
        UInt8* data = img->planes[0].data;
        UInt32 size;
        GetPlaneSize ( format, width, height, 1, &size );
        Int32 y = TABLE_WIDTH / 2 - 5;
        Int32 x = TABLE_HEIGHT / 2 - 5;
        Int32 length = 100;
        Int32 dir = 0; 
        Int32 r, c;
        for ( i = 0; i < numFrames; ++i )
        {
            memset ( data, 0, size );

            for ( r = y; r < y + length; ++r ) 
            {
                for ( c = x; c < x + length; ++c ) 
                {
                    data[r * img->planes[0].stride + c] = 255;
                }
            }

            y += dirY[dir];
            x += dirX[dir];
            if ( y < 0 || x < 0 || y + length >= IMG_WIDTH || x + length >= IMG_HEIGHT )
            {
                --dir;
                if ( dir < 0 )
                {
                    dir = 3;
                }
            }
            fwrite ( data, size, 1, fout );
        }

    }

    if ( fout != NULL )
    {
        fclose ( fout );
    }

    return status;

}