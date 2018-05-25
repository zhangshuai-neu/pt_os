#ifndef LIST_H
#define LIST_H

#include "stdint.h"

typedef struct list_node{
	struct list_node * prev;
	struct list_node * next;
}list_head;


#endif
