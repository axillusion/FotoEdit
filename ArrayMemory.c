#include <assert.h>
#include <stdio.h>
#include "ArrayMemory.h"

static void PrintStatistics()
{

}

static void Alloc()
{

}

static void Free()
{

}

Int32 ArrayMemory_Create ( 
   OUT ArrayMemory** arrMem ) 
{
   Int32 status = STATUS_OK;
   ArrayMemory* newArrMem;

   if ( arrMem == NULL )
   {
      status = STATUS_FAIL;
      printf ( "ArrayMemory_Create: Invalid input arguments\n" );
   }  

   if ( status == STATUS_OK )
   {
      newArrMem = malloc ( sizeof ( ArrayMemory ) );
      if ( newArrMem == NULL )
      {
         status = STATUS_FAIL;
         printf ( "Couldn't allocate %d data", ( Int32 ) sizeof ( ArrayMemory ) );
      }
   }
   
   if ( status == STATUS_OK )
   {
      newArrMem->vft->PrintStatistics = PrintStatistics;
      newArrMem->vft->Alloc = Alloc;
      newArrMem->vft->Free = Free;
      arrMem = newArrMem;
   }

   return status;
}