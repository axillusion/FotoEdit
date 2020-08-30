#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "Macros.h"
#include "Image.h"
#include "GenerateMovie.h"

Int32 GetImage ( Image* img, FILE* file ) 
{ 
    Int32 readStatus;

    if ( img == NULL || file == NULL )
    {
        readStatus = STATUS_FAIL;
        printf("GetImage: invalid input argument\n");
    } 
    else 
    {
        readStatus = CheckImage ( img, img->width, img->height, img->format );
    }

    if ( readStatus == STATUS_OK ) 
    {
        UInt32 dataSize;
        size_t chread;
        UInt8 numPlanes;
        Int32 i, stride;

        readStatus = GetNrPlanes ( img->format, &numPlanes );

        for ( i = 0; i < numPlanes && readStatus == STATUS_OK; ++i )
        {
            stride = img->planes[i].stride;
            readStatus = GetPlaneSize ( img->format, img->width, img->height, i + 1, &dataSize );
            if ( readStatus == STATUS_OK )
            {
                if ( img->width == stride )
                {
                    chread = fread ( img -> planes[i].data, dataSize, 1, file );
                    if ( chread != 1 )
                    {
                        readStatus = STATUS_FAIL;
                        printf ( "GetImage: Image not read\n" );
                    }
                } 
                else
                {
                    Int32 y, height, width;
                    height = img->height;
                    width = img->width;
                    if ( img->format == IMG_YUV420 && i > 0 )
                    {
                        height /= 2;
                        width /= 2;
                    }   

                    for ( y = 0; y < height && readStatus == STATUS_OK; ++y)
                    {
                        chread = fread ( img->planes[i].data + stride * y, width, 1, file );

                        if ( chread != 1 )
                        {
                            readStatus = STATUS_FAIL;
                            printf ( "GetImage: Image not read\n" );
                        }
                    }
                }
                
            }
        }
    }
    return readStatus;
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
// -i <path> -f <format> -s <subsampling> -w <width> -h <height> -b <bpp> -o <path>

void PrintUsage ( void ) 
{
    printf ( "Wrong usage\nUsage:\n-i <path> -s1 <subsampling> -w <width> -h <height> -b <bpp> -o <path> -s2 <subsampling>\n" );
}

Int32 InitFileAndImage ( 
    IN const char* path,
    IN Int32 forRead,
    IN UInt32 width,
    IN UInt32 height,
    IN UInt8 bpp,
    IN Int32 subsampling,
    OUT FILE** file,
    OUT Image** img )  
{
    UInt8 format;
    Int32 status = STATUS_OK;

    if ( path == NULL || file == NULL || img == NULL )
    {
        status = STATUS_FAIL;
        printf ( "InitFileAndImage: Invalid input parameters\n" );
    } 

    if ( status == STATUS_OK )
    {
        status = GetFormatFromPath ( path, subsampling, &format );
        if ( status != STATUS_OK )
        {
            printf ( "GetFormatFromPath failed with status %d", status );
        }
    }    

    if ( status == STATUS_OK )
    {   
        const char* openFlag = ( forRead == 0 ) ? "rb" : "wb";
        *file = fopen ( path, openFlag );
        if ( *file == NULL )
        {
            status = STATUS_FAIL;
            printf ( "InitImageAndFile: could not open file %s\n", path ); 
        }
    }

    if ( status == STATUS_OK )
    {
        status = CreateImage ( format, bpp, width, height, img );
        if (status != STATUS_OK )
        {
            printf("CreateImage failed with status %d", status);
        }
    }

    return status;
}

Int32 main ( Int32 argc, char* argv[] ) {
    
    Image* img;
    Image* outputImg;
    FILE *fin, *fout;

    char* inputPath = NULL;
    char* outputPath = NULL;
    Int32 i;
    Int32 width = 0;
    Int32 height = 0;
    Int32 bpp = 0;
    Int32 status = STATUS_OK;
    Int32 subsampling1 = 0;
    Int32 subsampling2 = 0;

    if ( argc != 15 ) 
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
                    case 's' :
                        ++arg;
                        if ( *arg == '1' )
                        {
                            subsampling1 = atoi ( argv[i + 1] );
                        }
                        else
                        {
                            subsampling2 = atoi ( argv[i + 1] );
                        }
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
        status = GenerateMovie ( width, height, 300, subsampling1, inputPath );
    }

    if ( status == STATUS_OK ) 
    {

        status = InitFileAndImage ( 
                 inputPath,
                 0, // for read
                 width,
                 height,
                 bpp,
                 subsampling1,
                 &fin,
                 &img );
    }

    if ( status == STATUS_OK )
    {
        status = InitFileAndImage ( 
                 outputPath,
                 1, // for write
                 width,
                 height,
                 bpp,
                 subsampling2,
                 &fout,
                 &outputImg );
    }
    i = 0;
    while ( GetImage ( img, fin ) == STATUS_OK ) 
    {
        printf ( "Proccesing frame %d\n", ++i);
        ConvertImage ( img, outputImg );
        WriteImageToFile ( outputImg, fout );
    }

    DestroyImage ( &img );
    DestroyImage ( &outputImg );

    if ( fin != NULL )
    {
        fclose ( fin );
    }
    if ( fout != NULL )
    {
        fclose ( fout );
    }

    return status;
}