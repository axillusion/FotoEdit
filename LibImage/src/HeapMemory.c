#include <assert.h>
#include <stdio.h>
#include <stdlib.h> // malloc 
#include <string.h> // memset
#include "HeapMemory.h"

static void* Alloc( HeapMemory* self, Int32 size )
{
   UInt8* addr = NULL;

   if ( self != NULL )
   {
      addr = malloc ( size + sizeof ( Int32 ) ); // + 4 bytes manarie
      if ( addr != NULL )
      {
         *( ( Int32* ) addr ) = size;
         addr += 4 ; // pastram primii 4 bytes ca sa tinem minte size-ul
      }
   }

   return addr;
}

static void Free ( HeapMemory* self, void* addr )
{
   if ( self != NULL )
   {
      Int32 size = *( ( ( Int32* ) addr ) - 1 );
      free ( ( ( Int32* ) addr ) - 1 );
   }
}

Int32 HeapMemory_Create ( 
   OUT HeapMemory** heapMem ) 
{
   Int32 status = STATUS_OK;
   HeapMemory* newHeapMem = NULL;

   if ( heapMem == NULL )
   {
      status = STATUS_FAIL;
      printf ( "ArrayMemory_Create: Invalid input arguments\n" );
   }  
   else 
   {
      *heapMem = NULL;
   }

   if ( status == STATUS_OK )
   {
      newHeapMem = malloc ( sizeof ( HeapMemory ) );
      if ( newHeapMem == NULL )
      {
         status = STATUS_FAIL;
         printf ( "Couldn't allocate %d data", ( Int32 ) sizeof ( HeapMemory ) );
      } 
      else
      {
         memset ( newHeapMem, 0, sizeof ( HeapMemory ) );
      }
      
   }
   
   if ( status == STATUS_OK )
   {
      newHeapMem->vft->PrintStatistics = PrintStatistics;
      newHeapMem->vft->Alloc = Alloc;
      newHeapMem->vft->Free = Free;

      newHeapMem->List = malloc ( sizeof ( Node ) );
      if ( newHeapMem->List == NULL )
      {
         status = STATUS_FAIL;
         printf ( "Couldn't allocate %d data", ( Int32 ) sizeof ( Node ) );
      }
      else 
      {
         memset ( newHeapMem->List, 0, sizeof ( Node ) );
         newHeapMem->List->next = newHeapMem->List;
         newHeapMem->List->prev = newHeapMem->List;
      }
   }

   if ( status != STATUS_OK )
   {
      if ( newHeapMem != NULL )
      {
         free ( newHeapMem );
      }
   }
   else 
   {
      *heapMem = newHeapMem;
   }

   return status;
}