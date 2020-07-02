#include <stdio.h>
#include "Macros.h"
#include "Image.h"

#define STATUS_NOT_READ -1
#define STATUS_READ 0
#define STATUS_EMPTY_IMG -1
#define STATUS_NOT_EMPTY_IMG 0

int GetImage ( Image* img, FILE* file ) 
{ 
    Int32 chread = fread ( &img -> data, img -> bpp, IMG_HEIGHT * IMG_WIDTH, file );

    Int32 readStatus;
    if ( chread != IMG_HEIGHT * IMG_WIDTH )
        readStatus = STATUS_NOT_READ;
    else 
        readStatus = STATUS_READ;

    return readStatus;
}

void ConvertImage ( Image* img ) 
{

}

Int32 PrintImage ( Image* img, FILE* file ) 
{ 
    Int32 status;

    if ( img == NULL ) 
    {
        printf ( "PrintImage: Empty image\n" );
        status = STATUS_EMPTY_IMG;
    } 
    else 
    {
        fwrite ( &img -> data, img -> bpp, IMG_HEIGHT * IMG_WIDTH, file );
        status = STATUS_NOT_EMPTY_IMG;
    }
    return status;
}

Int32 main ( Int32 argc, char* argv[] ) { 

    FILE* fin = fopen ( "video.in", "r" );

    Image* img;
    Int32 imgFormat = IMG_GRAY;
    CreateImage ( imgFormat, 8, IMG_WIDTH, IMG_HEIGHT, &img );

    FILE* fout = fopen ( "video.out", "w" );

    while ( GetImage ( &img, fin ) == IMG_WIDTH * IMG_HEIGHT ) {
        ConvertImage ( &img );
        PrintImage ( &img, fout );
        DestroyImage ( &img );
    }

    fclose ( fin );
    fclose ( fout );

    return 0;
}