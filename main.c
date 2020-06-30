#include <stdio.h>

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

int main ( int argc, char* argv[] ) {

    open_files();

    char **frame;

    while ( getFrame ( frame ) == width*height ) {
        convertFrame ( frame );
        printFrame ( frame );
    }

    return 0;
}