#ifndef ABSTRACT_MEMORY_H_INCLUDED
#define ABSTRACT_MEMORY_H_INCLUDED

#include "Memory.h"

typedef struct AbstractHeapMemory {
    // public fields
    struct AbstractHeapMemory_VFT* vft;
    // protected fields
    int peak;
    int usage;
} AbstractHeapMemory;

struct AbstractHeapMemory_VFT
{
    int ( *GetType ) ( AbstractHeapMemory* self );
    void* ( *Alloc ) ( AbstractHeapMemory* self, int size );
    void ( *Free ) ( AbstractHeapMemory* self, void* addr );
    void ( *PrintStatistics )( AbstractHeapMemory* self );
};

void PrintStatistics( AbstractHeapMemory* memObj );

#endif /* ABSTRACT_MEMORY_H_INCLUDED */