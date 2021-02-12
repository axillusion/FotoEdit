#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <LibImage.h>
#include "Defines.h"

Int32 GetImage ( Image* img, FILE* file ) 
{ 
    Int32 readStatus;
	UInt32 imgWidth, imgHeight, imgFormat;
	imgWidth = Image_GetWidth ( img );
	imgHeight = Image_GetHeight ( img );
	imgFormat = Image_GetFormat ( img );

    if ( img == NULL || file == NULL )
    {
        readStatus = STATUS_FAIL;
        printf("GetImage: invalid input argument\n");
    } 
    else 
    {
        readStatus = CheckImage ( img, imgWidth, imgHeight, imgFormat );
    }

    if ( readStatus == STATUS_OK ) 
    {
        UInt32 dataSize;
        UInt8 numPlanes;
        Int32 i, imgStride, imgData;
		UInt32 chread;

        readStatus = GetNrPlanes ( imgFormat, &numPlanes );

        for ( i = 0; i < numPlanes && readStatus == STATUS_OK; ++i )
        {
            imgStride = Image_GetPlaneStride ( img, i );
			imgData = Image_GetPlaneData ( img, i );
            readStatus = GetPlaneSize ( imgFormat, imgWidth, imgHeight, i + 1, &dataSize );
            if ( readStatus == STATUS_OK )
            {
                if ( imgWidth == imgStride )
                {
                    chread = fread ( imgData, dataSize, 1, file );
                    if ( chread != 1 )
                    {
                        readStatus = STATUS_FAIL;
                        printf ( "GetImage: Image not read\n" );
                    }
                } 
                else
                {
                    Int32 y;
                    if ( imgFormat == IMG_YUV420 && i > 0 )
                    {
                        imgHeight /= 2;
                        imgWidth /= 2;
                    }   

                    for ( y = 0; y < imgHeight && readStatus == STATUS_OK; ++y)
                    {
                        chread = fread ( imgData + imgStride * y, imgWidth, 1, file );

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

Int32 ExtractDataFromFilename ( 
    IN char* path,
    OUT Int32* width,
    OUT Int32* height,
    OUT Int32* bpp,
    OUT Int32* subsampling )
{

    Int32 status = STATUS_OK;
    char *localpath = strdup ( path );

    if ( localpath != NULL )
    {
        path = localpath;
    }

    assert ( path != NULL );
    assert ( width != NULL );
    assert ( height != NULL );
    assert ( bpp != NULL );
    assert ( subsampling != NULL );

    char* p = strtok_r ( path, "_", &path );

    /*
        File_2020_09_02_640x480_b8_30Hz_P400.bw
        _
    */

    while ( p != NULL )
    {
        if ( strchr ( p, 'x' ) != NULL )
        {
            char* strWidth = strtok_r ( p, "x", &p );   
            char* strHeight = strtok_r ( p, "x", &p );
            if ( strWidth != NULL && strHeight != NULL )
            {   
                Int32 w = atoi ( strWidth );
                Int32 h = atoi ( strHeight );

                if ( *width != 0 && *width != w )
                {
                    printf ( "Warning: Command line argument -w [ %d ] overwrites parsed -w [ %d ] from file name\n", *width, w );
                }
                else 
                {
                    *width = w;
                }

                if ( *height != 0 && *height != h )
                {
                    printf ( "Warning: Command line argument -h [ %d ] overwrites parsed -h [ %d ] from file name\n", *height, h );
                }
                else 
                {
                    *height = h;
                }
            }
        }
        else if ( p[0] == 'b' )
        {
            if ( strlen ( p ) > 1 )
            {
                Int32 b = atoi ( p + 1 );

                if ( *bpp != 0 && *bpp != b )
                {
                    printf ( "Warning: Command line argument -b [ %d ] overwrites parsed -b [ %d ] from file name\n", *bpp, b );
                }
                else 
                {
                    *bpp = b;
                }
            }
        }
        else if ( p[0] == 'P' )
        {
            if ( strlen ( p ) >= 4 )
            {
                Int32 s = atoi ( p + 1 );
                printf ( "%d\n", s );

                if ( *subsampling != 0 && *subsampling != s )
                {
                    printf ( "Warning: Command line argument -s [ %d ] overwrites parsed -s [ %d ] from file name\n", *subsampling, s );
                }
                else 
                {
                    *subsampling = s;
                }
            }
        }

        p = strtok_r ( path, "_", &path );
    }

    if ( *width == 0 || *height == 0 || *bpp == 0 || *subsampling == 0 )
    {
        status = STATUS_FAIL;
        printf ( "%d %d %d %d\n", *width, *height, *subsampling, *bpp );
        PrintUsage();
    }

    free ( localpath );

    return status;
}

Int32 main ( Int32 argc, char* argv[] ) {
    
    Image* img;
    Image* outputImg;
    FILE *fin = NULL;
    FILE *fout = NULL;

    char* inputPath = NULL;
    char* outputPath = NULL;

    Int32 width = 0;
    Int32 height = 0;
    Int32 bpp = 0;
    Int32 inputSubsampling = 0;
    Int32 outputSubsampling = 0;

    Int32 status = STATUS_OK;
    Int32 i;
    /*
        File_2020_09_02_640x480_b8_30Hz_P400.bw
        Parseaza argumentele de intrare
        Parseaza path-ul de intrare si iesire pentru parametrii
    */

    //HeapMemory* heapMem; 
    //status = HeapMemory_Create ( &heapMem );

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
                        inputSubsampling = atoi ( argv[i + 1] );
                    }
                    else
                    {
                        outputSubsampling = atoi ( argv[i + 1] );
                    }
                    break;
                default : 
                    printf ( "1\n");
                    PrintUsage();
                    status = STATUS_FAIL;
            }
        } 
        else
        {
            printf ( "2\n");
            PrintUsage();
            status = STATUS_FAIL;
        } 
    }

    if ( inputPath == NULL || outputPath == NULL )
    {
        printf ( "3\n");
        PrintUsage();
        status = STATUS_FAIL; 
    }

    if ( status == STATUS_OK )
    {
        status = ExtractDataFromFilename ( 
                    inputPath,
                    &width,
                    &height,
                    &bpp,
                    &inputSubsampling );
    }

    if ( status == STATUS_OK )
    {
        status = ExtractDataFromFilename ( 
                    outputPath,
                    &width,
                    &height,
                    &bpp,
                    &outputSubsampling );
    }

    if ( status == STATUS_OK ) 
    {
        status = GenerateMovie ( width, height, 300, inputSubsampling, inputPath );
    }

    if ( status == STATUS_OK ) 
    {

        status = InitFileAndImage ( 
                 inputPath,
                 0, // for read
                 width,
                 height,
                 bpp,
                 inputSubsampling,
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
                 outputSubsampling,
                 &fout,
                 &outputImg );
    }

    if ( status == STATUS_OK )
    {
        i = 0;
        while ( GetImage ( img, fin ) == STATUS_OK ) 
        {
            printf ( "Proccesing frame %d\n", ++i);
            ConvertImage ( img, outputImg );
            WriteImageToFile ( outputImg, fout );
        }

        DestroyImage ( &img );
        DestroyImage ( &outputImg );
    }

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