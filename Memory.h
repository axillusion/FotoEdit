#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

/*
Facem vft pentru Memory + structura
ArrayMemory + Heap memory fisiere separate .c + .h cu implementare
*/

struct IMemory_VFT
{
    int ( *GetType ) ( void );
    void* ( *Alloc ) ( int size );
    void ( *Free ) ( void* addr );
    
};

typedef struct IMemory {
    struct IMemory_VFT* vft;
} IMemory;

#endif // MEMORY_H_INCLUDED
