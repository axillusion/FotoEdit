#include "AbstractMemory.h"

void PrintStatistics( AbstractHeapMemory* memObj )
{
    printf ( "Peak: %d bytes\nUsage: %d bytes\n", memObj->peak, memObj->usage );
}