#include <assert.h>
#include <stdio.h>
#include <stdlib.h> // malloc 
#include <string.h> // memset

#include "LibImage.h"
#include "List.h"
#include "HeapMemory.h"

typedef struct HeapMemory 
{
    struct IMemoryVFT* vft;
    
    UInt32 peak;
    UInt32 usage;
   
	Node* List;
    
} HeapMemory;

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
   OUT IMemory** memory ) 
{
   Int32 status = STATUS_OK;
   IMemory* newMemory = NULL;

   if ( memory == NULL )
   {
      status = STATUS_FAIL;
      printf ( "HeapMemory_Create: Invalid input arguments\n" );
   }  
   else 
   {
      *memory = NULL;
   }

   if ( status == STATUS_OK )
   {
      newMemory = malloc ( sizeof ( IMemory ) );
      if ( newMemory == NULL )
      {
         status = STATUS_FAIL;
         printf ( "Couldn't allocate %d data", ( Int32 ) sizeof ( IMemory ) );
      } 
      else
      {
         memset ( newMemory, 0, sizeof ( IMemory ) );
      }
      
   }
   
   if ( status == STATUS_OK )
   {
      //newMemory->vft->PrintStatistics = PrintStatistics;
      newMemory->vft->Alloc = Alloc;
      newMemory->vft->Free = Free;
	  /*
      newMemory->List = malloc ( sizeof ( Node ) );
      if ( newMemory->List == NULL )
      {
         status = STATUS_FAIL;
         printf ( "Couldn't allocate %d data", ( Int32 ) sizeof ( Node ) );
      }
      else 
      {
         memset ( newMemory->List, 0, sizeof ( Node ) );
         newMemory->List->next = newMemory->List;
         newMemory->List->prev = newMemory->List;
      }
	  */
   }

   if ( status != STATUS_OK )
   {
      if ( newMemory != NULL )
      {
         free ( newMemory );
      }
   }
   else 
   {
      *memory = newMemory;
   }

   return status;
}