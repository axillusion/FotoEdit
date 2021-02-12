#ifndef HEAP_MEMORY_H_INCLUDED
#define HEAP_MEMORY_H_INCLUDED

#include "Defines.h"
#include "AbstractMemory.h"

typedef struct Node
{
   Int32 size;
   void* addr;

   struct Node* next;
   struct Node* prev;
} Node;

typedef struct HeapMemory {
    // public fields
    struct HeapMemory_VFT* vft;
    // protected fields
    UInt32 peak;
    UInt32 usage;
    //private fields 
	Node* List;
    //Node* List;
} HeapMemory;

struct HeapMemory_VFT
{
    int ( *GetType ) ( HeapMemory* self );
    void* ( *Alloc ) ( HeapMemory* self, int size );
    void ( *Free ) ( HeapMemory* self, void* addr );
    void ( *PrintStatistics )( AbstractHeapMemory* self );
};

Int32 HeapMemory_Create ( 
    OUT HeapMemory** heapMem
);

#endif /* ARRAY_MEMORY_H_INCLUDED */