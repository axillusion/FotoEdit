#ifndef ARRAY_MEMORY_H_INCLUDED
#define ARRAY_MEMORY_H_INCLUDED

#include "Defines.h"

struct ArrayMemory_VFT
{
    int ( *GetType ) ( void );
    void* ( *Alloc ) ( int size );
    void ( *Free ) ( void* addr );
    void ( *PrintStatistics )();
};

typedef struct ArrayMemory {
    // public fields
    struct ArrayMemory_VFT* vft;
    // private fields
    int m_size;
} ArrayMemory;

Int32 ArrayMemory_Create ( 
    OUT ArrayMemory** arrMem
);

#endif /* ARRAY_MEMORY_H_INCLUDED */