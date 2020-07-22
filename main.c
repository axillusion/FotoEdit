#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
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

        UInt32 dataSize;
        
        readStatus = GetImageSize ( img, &dataSize );

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
// -i <path> -f <format> -w <width> -h <height> -b <bpp> -o <path>

void PrintUsage ( void ) 
{
    printf ( "Wrong usage\nUsage:\n-i <path> -f <format> -w <width> -h <height> -b <bpp> -o <path>\n" );
}

Int32 main ( Int32 argc, char* argv[] ) {
    
    Image* img;

    char* inputPath = NULL;
    char* outputPath = NULL;
    Int32 i;
    Int32 width = 0;
    Int32 height = 0;
    UInt8 format = 0;
    Int32 bpp = 0;
    Int32 status = STATUS_OK;

    if ( argc != 13 ) 
    {
        PrintUsage();
        status = STATUS_FAIL;
    }
    else
    {
        for ( i = 1; i < argc && status == STATUS_OK; i += 2 )
        {
            const char* arg = argv[i];

            if ( *arg == '-' && ( i + 1 ) < argc )  
            {
                ++arg;
                switch ( *arg )
                {
                    case 'i' :
                        inputPath = argv[i + 1];
                        break;
                    case 'f' :
                        format = ( UInt8 ) atoi ( argv[i + 1] );
                        break;
                    case 'w' :
                        width = atoi ( argv[i + 1] );
                        break;
                    case 'h' :
                        height = atoi ( argv[i + 1] );
                        break;
                    case 'b' :
                        bpp = atoi ( argv[i + 1] );
                        break;
                    case 'o' :
                        outputPath = argv[i + 1];
                        break;
                    default : 
                        PrintUsage();
                        status = STATUS_FAIL;
                }
            } 
            else
            {
                PrintUsage();
                status = STATUS_FAIL;
            } 
        }
    }

    if ( status == STATUS_OK ) 
    {
        FILE* fin = fopen ( inputPath, "rb" );
        
        Int32 status;
        status = CreateImage ( format, bpp, width, height, &img );

        if (status != STATUS_OK )
        {
            printf("CreateImage failed with status %d", status);
        }

        FILE* fout = fopen ( outputPath, "wb" );

        while ( GetImage ( img, fin ) == STATUS_READ ) {
            ConvertImage ( img );
            PrintImage ( img, fout );
        }

        DestroyImage ( &img );

        if ( fin != NULL )
        {
            fclose ( fin );
        }
        if ( fout != NULL )
        {
            fclose ( fout );
        }
    }

    return status;
}