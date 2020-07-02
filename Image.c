#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Macros.h"
#include "Image.h"

Int32 CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img)
{
    Int32 status;

    if ( img == NULL ) 
    {
        printf ( "CreateImage: invalid pointer!\n" );
        status = STATUS_FAIL;
    } 
    else 
    {
        Image* newImg;

        newImg = malloc ( sizeof ( Image ) );
        if ( newImg == NULL ) 
        {
            printf ( "CreateImage: could not allocate %d bytes\n", sizeof ( Image ) );
            status = STATUS_FAIL;
        }
        else 
        {
            UInt32 dataSize = width * height * ( ( bpp + 7 ) >> 3 );
            memset ( newImg, 0, sizeof ( Image ) );

            newImg -> data = malloc ( dataSize );

            if ( newImg -> data == NULL ) 
            {
                printf ( "CreateImage: could not allocate %d bytes\n", dataSize );
                status = STATUS_FAIL;
            }
            else 
            {   
                memset ( newImg -> data, 0, dataSize );
                newImg -> format = format;
                newImg -> bpp = bpp;
                newImg -> width = width;
                newImg -> height = height;
                *img = newImg;

                status = STATUS_OK;
            }
        }
    }

    return status;
}

void DestroyImage (
    IN OUT Image** img ) 
{
    if ( img == NULL ) 
    {
        printf ( "DestroyImage: Missing image" );
        return ;
    }
    free ( *img );
}

    