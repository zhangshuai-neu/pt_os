/*
 * 双向循环链表
 * 
 * list_node应当放入需要的管理的new_struct内部
 * 在管理list_node时，new_struct自然的被串联起来
 *
 * author:Shuai Zhang
 * email:zhangshuaiisme@gmail.com
 */

#ifndef LIST_H
#define LIST_H

#include "stdint.h"

typedef struct list_node{
	struct list_node * prev;
	struct list_node * next;
}list_head;

//链表节点
typedef struct list_node{
	struct list_node *next, *prev;
}list_node;

/*
 * 
 * 用法：list_entry(&list_node,struct page,list_node)
 * */
#define list_entry(link, type, member) \
	((type *)((char *)(link)-(unsigned long)(&((type *)0)->member)))

#endif
