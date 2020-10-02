#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

/*
Facem vft pentru Memory + structura
ArrayMemory + Heap memory fisiere separate .c + .h cu implementare
*/

typedef struct IMemory {
    struct IMemory_VFT* vft;
} IMemory;

struct IMemory_VFT
{
    int ( *GetType ) ( IMemory* self );
    void* ( *Alloc ) ( IMemory* self, int size );
    void ( *Free ) ( IMemory* self, void* addr );
    
};

#endif // MEMORY_H_INCLUDED
