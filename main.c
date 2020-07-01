#include <stdio.h>
#include <Image.h>

int GetImage ( Image* img, FILE* file ) { 
    return fread ( &img -> data, img -> bpp, IMG_HEIGHT * IMG_WIDTH, file );
}

void ConvertImage ( Image* img ) {

}

int PrintImage ( Image* img, FILE* file ) { 
    return fwrite ( &img -> data, img -> bpp, IMG_HEIGHT * IMG_WIDTH, file );
}

int main ( int argc, char* argv[] ) { 

    FILE* fin = fopen ( "video.in", "r" );

    Image* img;
    CreateImage ( 1, 8, IMG_WIDTH, IMG_HEIGHT, &img );

    FILE* fout = fopen ( "video.out", "w" );

    while ( GetImage ( &img, fin ) == IMG_WIDTH * IMG_HEIGHT ) {
        convertImage ( &img );
        printImage ( &img, fout );
    }

    return 0;
}