
#include "list.h"
#include "stdint.h"

void list_init(list_head *list){
	list->next = list->prev = list;
}

