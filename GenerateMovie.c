#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "GenerateMovie.h"

#define TABLE_WIDTH 1024
#define TABLE_HEIGHT 1024

/* C:/output/video1.bw */

Int32 GenerateMovie (
    IN Int32 width,
    IN Int32 height,
    IN Int32 numFrames,
    IN const char* path )
{
    Int32 status = STATUS_OK;
    UInt8 format;
    Int32 i;
    FILE* fout = NULL;
    Image* img;

    srand( time ( NULL ) );

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
        status = GetFormatFromPath ( path, &format );
    }

    if ( status == STATUS_OK )
    {
        status = CreateImage ( format, 8, width, height, &img ); 
    }

    if ( status == STATUS_OK ) 
    {
        UInt32 size;
        Int32 y = img->width / 2 - 5;
        Int32 x = img->height / 2 - 5;
        Int32 dirY[] = { -1, 0, 1, 0, -1, -1, 1, 1 };
        Int32 dirX[] = { 0, 1, 0, -1, -1, 1, 1, -1 };
        Int32 length = 10;
        Int32 dir = rand() % 8; 
        Rectangle* rectangle = malloc ( sizeof ( Rectangle ) );
        Int32 step = img->width / 10;

        GetPlaneSize ( img->format, img->width, img->height, 1, &size );
        rectangle->width = length;
        rectangle->height = length;

        for ( i = 0; i < numFrames && status == STATUS_OK; ++i )
        {   
            status = ClearImage ( img, 0x000000 );

            if ( status == STATUS_OK )
            {
                rectangle->top = y;
                rectangle->left = x;
                status = DrawRectangle ( img, rectangle, 0xFF0000 );
            }

            if ( status == STATUS_OK )
            {
                y += dirY[dir] * step;
                x += dirX[dir] * step;
                while ( y < 0 || x < 0 || y + length >= img->width || x + length >= img->height )
                {
                    y -= dirY[dir] * step;
                    x -= dirX[dir] * step;
                    dir = rand() % 8;
                    y += dirY[dir] * step;
                    x += dirX[dir] * step;
                }
                status = WriteImageToFile ( img, fout );
            }
        }

    }

    if ( fout != NULL )
    {
        fclose ( fout );
    }

    return status;
}

Int32 GetFormatFromPath ( 
    IN const char* path,
    OUT UInt8* format )
{
    Int32 status, lastDotPos, i;

    if ( path == NULL || format == NULL )
    {
        status = STATUS_FAIL;
        printf ( "GetFormatFromPath: invalid input / output arguments\n" );
    }
    else 
    {
        
        lastDotPos = -1;
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
            printf ( "GetFormatFromPath: Invalid path\n" );
        } 
        else
        {
            const char* ext = path + lastDotPos + 1;
            status = STATUS_OK;
            if ( strcmp ( ext, "bw" ) == 0 )
            {
                *format = IMG_GRAY;
            } 
            else if ( strcmp ( ext, "rgb" ) == 0 )
            {
                *format = IMG_RGB;
            } 
            else if ( strcmp ( ext, "yuv" ) == 0 ) 
            {
                *format = IMG_YUV;
            } 
            else
            {
                status = STATUS_FAIL;
                printf ( "GetFormatFromPath: Invalid file format\n" );
            }
        }
    }

    return status;
}
