#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#define IMG_GRAY 1
#define IMG_RGB 2
#define IMG_YUV420 3
#define IMG_YUV444 4

typedef struct ImgPlane ImgPlane;
typedef struct Image Image; 

/*------------------------------------------------------------------------------
 * Image_GetWidth
 *----------------------------------------------------------------------------*/
/** 
    Returns the width of a specified Image object.

@param [in] img
    pointer to the Image object
@return 
	the width of the Image object
@notes 
	returns -1 if the Image pointer is invalid
 ******************************************************************************/

Int32 Image_GetWidth ( 
    IN const Image *img );

/*------------------------------------------------------------------------------
 * Image_GetHeight
 *----------------------------------------------------------------------------*/
/** 
    Returns the height of a specified Image object.

@param [in] img
    pointer to the Image object
@return 
	the height of the Image object
@notes 
	returns -1 if the Image pointer is invalid
 ******************************************************************************/

Int32 Image_GetHeight ( 
    IN const Image *img );

/*------------------------------------------------------------------------------
 * Image_GetBPP
 *----------------------------------------------------------------------------*/
/** 
    Returns the BPP ( bits per pixel ) of a specified Image object.

@param [in] img
    pointer to the Image object
@return 
	the bpp of the Image object
@notes 
	returns -1 if the Image pointer is invalid
 ******************************************************************************/

Int8 Image_GetBPP ( 
    IN const Image *img );

/*------------------------------------------------------------------------------
 * Image_GetFormat
 *----------------------------------------------------------------------------*/
/** 
    Returns the format of a specified Image object.

@param [in] img
    pointer to the Image object
@return 
	the format of the Image object
@notes 
	returns -1 if the Image pointer is invalid
 ******************************************************************************/

Int8 Image_GetFormat ( 
    IN const Image *img );

/*------------------------------------------------------------------------------
 * Image_GetNumPlanes
 *----------------------------------------------------------------------------*/
/** 
    Returns the number of planes of a specified Image object.

@param [in] img
    pointer to the Image object
@return 
	the number of planes of the Image object
@notes 
	returns -1 if the Image pointer is invalid
 ******************************************************************************/

Int8 Image_GetNumPlanes(
    IN const Image *img );

/*------------------------------------------------------------------------------
 * Image_GetPlaneStride
 *----------------------------------------------------------------------------*/
/** 
    Returns the stride of a specified plane from an Image object.

@param [in] img
    pointer to the Image object
@param [in] plane
	plane number
@return 
	the stride of the specified plane from the Image object
@notes 
	returns -1 if the Image pointer is invalid
 ******************************************************************************/

Int32 Image_GetPlaneStride (
    IN const Image *img,
    IN const UInt8 plane );

/*------------------------------------------------------------------------------
 * Image_GetPlaneData
 *----------------------------------------------------------------------------*/
/** 
    Returns the data of a specified plane from an Image object.

@param [in] img
    pointer to the Image object
@param [in] plane
	plane number
@return 
	the data of the specified plane from the Image object
@notes 
	returns NULL if the Image pointer is invalid
 ******************************************************************************/

void* Image_GetPlaneData (
    IN const Image *img,
    IN const UInt8 plane );

/*------------------------------------------------------------------------------
 * Image_CreateImage
 *----------------------------------------------------------------------------*/
/** 
    Creates an Image object with the specified format, bpp ( bits per pixel )
	width and height

@param [in] format
    format of the image
@param [in] bpp
	bpp of the image
@param [in] width
	width of the image
@param [in] height
	height of the image
@param [out] img 
	pointer to the Image object 
@return 
	the status of the functions, STATUS_FAIL if it encountered issues or 
	STATUS_OK if everything is completed
 ******************************************************************************/

Int32 Image_Create(
    IN IMemory* heap,
	IN UInt8 format,
    IN UInt8 bpp,
    IN UInt32 width,
    IN UInt32 height,
    OUT Image** img );
    
/*------------------------------------------------------------------------------
 * Image_DestroyImage
 *----------------------------------------------------------------------------*/
/** 
    Destroys the specified Image object

@param [in][out] img 
	pointer to the Image object 

 ******************************************************************************/

void Image_Destroy(
    IN OUT Image** img);

Int32 Image_Crop (
    IN const Image* img,
    IN UInt32 cropWidth,
    IN UInt32 cropHeight,
    IN UInt32 offsetX,
    IN UInt32 offsetY, 
    OUT Image* crop );

Int32 Image_GetPlaneSize(
    IN UInt8 format,
    IN UInt32 width,
    IN UInt32 height,
    IN UInt8 plane,
    OUT UInt32* planeSize);
    
Int32 Image_GetImageSize(
    IN Image* img,
    OUT UInt32* size);

Int32 Image_Check(
    IN const Image* img,
    IN UInt32 requiredWidth,
    IN UInt32 requiredHeight,
    IN UInt8 requiredFormat);

Int32 Image_Clear (
    IN OUT Image* img,
    UInt32 color );

Int32 Image_WriteImageToFile ( 
    IN const Image* img,
    IN FILE* file );

Int32 Image_Convert ( 
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
/*
struct IMemory_VFT
{
    int ( *GetType ) ( void );
    void* ( *Alloc ) ( int size );
    void ( *Free ) ( void* addr );
};

typedef struct IMemory {
    struct IMemory_VFT* vft;
} IMemory;
*/
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
/*
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
*/
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
/*
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
*/
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
/*
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
*/