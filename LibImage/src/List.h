#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "LibImage.h"

typedef struct Node
{
   Int32 size;
   void* addr;

   struct Node* next;
   struct Node* prev;
} Node;

#endif /* LIST_H_INCLUDED */
