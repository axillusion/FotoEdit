#include <assert.h>
#include <stdio.h>
#include "HeapMemory.h"

typedef struct Node
{
   Int32 size;
   void* addr;

   Node* next;
   Node* prev;
} Node;

static void* Alloc( HeapMemory* self, Int32 size )
{
   void* addr = NULL;

   if ( self != NULL )
   {
      Node* node = malloc ( sizeof ( Node ) );

      if ( node != NULL )
      {
         memset ( node, 0, sizeof ( Node ) );
         addr = malloc ( size );

         if ( addr != NULL )
         {
            node->size = size;
            node->addr = addr;
            self->usage += size;
            self->peak = ( self->peak < self->usage ) ? self->usage : self->peak; 
            node->next = self->List->next;
            node->prev = self->List;
            self->List->next = node;
         } 
         else 
         {
            free ( node );
         }
      } 
   }

   return addr;
}

static void Free ( HeapMemory* self, void* addr )
{
   if ( self != NULL && self->List != NULL )
   {
      Node* node = self->List->next;

      while ( node != self->List && node->addr != addr )
      {
         node = node->next;
      }

      if ( node != self->List )
      {
         node->prev->next = node->next;
         node->next->prev = node->prev;
         self->usage -= node->size;
         free ( node->addr );
         free ( node );
      } 
      else
      {
         printf ( "Address not allocated by this Heap object\n" );
      }
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