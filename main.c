#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "Macros.h"
#include "Image.h"

#define STATUS_NOT_READ -1
#define STATUS_READ 0
#define STATUS_EMPTY_IMG -1
#define STATUS_NOT_EMPTY_IMG 0
#define STATUS_UNALLOCATED_IMG -2


int GetImage ( Image* img, FILE* file ) 
{ 
    Int32 readStatus = STATUS_OK;

    if (img == NULL)
    {
        readStatus = STATUS_UNALLOCATED_IMG;
        printf("GetImage: unalocated image\n");
    }
    else if ( img->planes == NULL )
    {
        readStatus = STATUS_UNALLOCATED_IMG;
        printf ( "GetImage: unalocated plane\n" );
    }
    else if (file == NULL)
    {
        readStatus = STATUS_UNALLOCATED_IMG;
        printf("GetImage: invalid file descriptor\n");
    }
    else
    {
        // incomplet, nu se tine cont de formatul imaginii si de bpp

        Int32 dataSize;
        
        switch ( img->format ) 
        {
            case IMG_GRAY :
                dataSize = img->width * img->height *  ( ( img->bpp + 7 ) >> 3 );
                break;
            case IMG_RGB :
                dataSize = img->width * img->height *  ( ( img->bpp + 7 ) >> 3 ) * 3;
                break;
            case IMG_YUV :
                dataSize = img->width * img->height *  ( ( img->bpp + 7 ) >> 3 ) * 2;
                break;
            default :
                dataSize = 0;
                printf ( "GetImage: Invalid image format\n" );
                readStatus = STATUS_NOT_READ;
                break;
        }

        if ( readStatus == STATUS_OK )
        {
            size_t chread = fread ( &img -> planes, img -> bpp, dataSize, file );

            if ( chread != dataSize )
            {
                readStatus = STATUS_NOT_READ;
            }
        }
    }
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
        fwrite ( &img -> planes, img -> bpp, IMG_HEIGHT * IMG_WIDTH, file );
        status = STATUS_NOT_EMPTY_IMG;
    }
    return status;
}

/*
baza 2      = binara = 0, 1
baza 10     = decima = [0, 9]
baza 16     = hexa = [0, F]


8 biti = 255

11111111b
255
0xFF




|0x11| Image
HEAP (RAM)


    UInt32 width;
    UInt32 height;
    UInt8* data;
    UInt8 bpp;
    UInt8 format;


|    |
|0x10| main, sub sp, 0x11
|0x0F| img.width
|0x0B| img.height          
|0x09| img.data
|0x08| img.bpp
|0x07| img.format 
|0x06| img
|0x05| fin, sp
|    |
|0x01| sp
+0x00+ NULL


32bit = 0xFFFFFFFF = 4.294.967.295 4GB

sp = 0x01
sp += 0x0F = 0x10 (add sp, 0xF)
call main (jmp 0x10)

addr = NULL
adrr = 0xFF

main = 0x10
img = 0

*/


// verifica argumetele de intrare si tine cont de ele cand creem imaginea de intrare si imaginea de iesire

// Usage:
// -i <path> -f <format> -w <width> -h <height> -b <bpp> -o <path> help

Int32 main ( Int32 argc, char* argv[] ) {
    
    Image* img;
    
    FILE* fin = fopen ( "video.in", "rb" );


    Int32 status;
    status = CreateImage (IMG_GRAY, 8, IMG_WIDTH, IMG_HEIGHT, &img );

    if (status != STATUS_OK )
    {
        printf("CreateImage failed with status %d", status);
    }

    FILE* fout = fopen ( "video.out", "wb" );

    while ( GetImage ( img, fin ) == STATUS_READ ) {
        ConvertImage ( img );
        PrintImage ( img, fout );
    }

    DestroyImage ( &img );

    fclose ( fin );
    fclose ( fout );

    return 0;
}