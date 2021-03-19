#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

/*
Facem vft pentru Memory + structura
ArrayMemory + Heap memory fisiere separate .c + .h cu implementare
*/

/**
 * @file
 */

typedef struct IMemory {
    struct IMemoryVFT* vft;
} IMemory;

struct IMemoryVFT
{
    void* ( *Alloc ) ( 
		IN IMemory* memory, 
		IN UInt32 size );
	
    void ( *Free ) ( 
		IN IMemory* memory, 
		IN void* addr );
		
	void ( *Destroy ) ( 
		IN OUT IMemory* memory );
};

#endif // MEMORY_H_INCLUDED
