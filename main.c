#include <stdio.h>

#define IN
#define OUT

typedef signed char Int8;
typedef signed short Int16;
typedef signed int Int32;


typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned int UInt32;

#define IMG_WIDTH 100
#define IMG_HEIGHT 100



// Image.h

typedef struct Image
{
    UInt32 width;
    UInt32 height;
    UInt8* data;
    UInt8 bpp;
    UInt8 format;
    
} Image;

int CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img);
    
void DestroyImage(
    IN OUT Image** img);
    
// Image.c
int CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image* img)
{
}


FILE *fin, *fout;

int width;
int height;
size_t bytes;
char format;

void open_files () {
    fin = fopen ( "video_in.bw", "rb" );
    fout = fopen ( "video_out.bw", "wb" );
}

int getFrame ( char **frame ) { 
    return fread ( &frame, bytes, width*height, fin );
}

void convertFrame ( char **frame ) {

}

void printFrame ( char **frame ) {
    fwrite ( frame, bytes, width*height, fout );
}

gcc main.c image.c -o exec

int main ( int argc, char* argv[] ) {

    Image* img = NULL;
    CreateImage(&img);

    
    UInt8* img;
    img = malloc(IMG_WIDTH * IMG_HEIGHT * sizeof(UInt8));
    
    for(int y = 0; y < IMG_HEIGHT; y++)
    {
        for(int x = 0; x < IMG_WIDTH; x++)
        {
            UInt8 val = img[y * IMG_WIDTH + x];
        }
    }
    

    
    

    open_files();

    char** frame;

    while ( getFrame ( frame ) == width*height ) {
        convertFrame ( frame );
        printFrame ( frame );
    }

    return 0;
}