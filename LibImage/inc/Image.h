#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "GenerateMovie.h"

#define IMG_GRAY 1
#define IMG_RGB 2
#define IMG_YUV420 3
#define IMG_YUV444 4

#define MAX_NUM_PLANES 3

typedef struct Rectangle
{
    Int32 top, left, width, height;
} Rectangle;

typedef struct ImgPlane {
    void* data;
    UInt32 stride;
} ImgPlane;

typedef struct Image Image; 

UInt32 Image_GetWidth ( 
    IN Image *img );

UInt32 Image_GetHeight ( 
    IN Image *img );

UInt8 Image_GetBPP ( 
    IN Image *img );

UInt8 Image_GetFormat ( 
    IN Image *img );

UInt8 Image_GetNumPlanes (
    In Image *img );

UInt8 Image_GetPlaneStride (
    IN Image *img,
    IN UInt8 plane );

void* Image_GetPlaneData (
    IN Image *img,
    IN UInt8 plane );

Int32 CreateImage(
    IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img );
    
void DestroyImage(
    IN OUT Image** img);

Int32 CropImage (
    IN const Image* img,
    IN UInt32 cropWidth,
    IN UInt32 cropHeight,
    IN UInt32 offsetX,
    IN UInt32 offsetY, 
    OUT Image* crop );

Int32 GetPlaneSize(
    IN UInt8 format,
    IN UInt32 width,
    IN UInt32 height,
    IN UInt8 plane,
    OUT UInt32* planeSize);

Int32 GetNrPlanes(
    IN UInt8 format,
    OUT UInt8* numPlanes);
    
Int32 GetImageSize(
    IN Image* img,
    OUT UInt32* size);

Int32 CheckImage(
    IN const Image* img,
    IN UInt32 requiredWidth,
    IN UInt32 requiredHeight,
    IN UInt8 requiredFormat);

Int32 ClearImage (
    IN OUT Image* img,
    UInt32 color );

Int32 WriteImageToFile ( 
    IN const Image* img,
    IN FILE* file );

Int32 ConvertImage ( 
    IN const Image* src,
    OUT Image* dst );

#endif /* IMAGE_H_INCLUDED */

/* 
support for format yuv444
CreateImage
DestroyImage
NumPLane
PlaneSize
Draw
Convert
Sampling - 444 ( -s )


class IMemory 
{
    public :
        virtual void* Alloc ( int size ) = 0;
        virtual void Free ( void* addr ) = 0;
};*/

struct IMemory_VFT
{
    int ( *GetType ) ( void );
    void* ( *Alloc ) ( int size );
    void ( *Free ) ( void* addr );
};

typedef struct IMemory {
    struct IMemory_VFT* vft;
} IMemory;

/*
class AbstractMemory: public IMemory
{
    public : 
        virtual void PrintStatistics()
        {
            printf ( "%d\n", m_size );
        }
    protected :
        int m_size;
};*/

struct AbstractMemory_VFT {
    int ( *GetType ) ( void );
    void* ( *Alloc ) ( int size );
    void ( *Free ) ( void* addr );
    void ( *PrintStatistitcs ) ();
};

typedef struct AbstractMemory {
    struct AbstractMemory_VFT* vft;
    int m_size;
};

/*
class HeapMemory: public AbstractMemory 
{
    public :
        virtual void* Alloc ( int size )
        {

        }

        virtual void Free ( void* addr )
        {

        }
};*/

typedef struct HeapMemory {
    int m_size;
    void* ( *Alloc ) ( int size );
    void ( *Free ) ( void* addr );
    void ( *PrintStatistics )();
} HeapMemory;

typedef struct ArrayMemory {
    void* ( *Alloc ) ( int size );
    void ( *Free ) ( void* addr );
    void ( *PrintStatistics )();
    int m_size;
} ArrayMemory;
/*
class ArrayMemory: public AbstractMemory 
{
    public :
        virtual void* Alloc ( int size )
        {

        }

        virtual void Free ( void* addr )
        {

        }
};

class Image 
{
    public : 
        Image ( IMemory* memory )
        {
            memory->Alloc ( sizeof ( Image ) );
        }
        
};*/

void ImageCreate ( IMemory *memory )
{
    ArrayMemory *absMem = ( ArrayMemory* ) memory;
    memory->vft->Alloc ( sizeof ( Image ) );
    absMem->PrintStatistics();
}

void* HeapAlloc ( int size )
{
    return NULL;
}

void Init()
{
    HeapMemory *heap = ( HeapMemory* ) malloc ( sizeof ( HeapMemory ) );
    memset ( heap, 0, sizeof ( HeapMemory ) );
    heap->Alloc = HeapAlloc;
    ImageCreate ( heap );
}