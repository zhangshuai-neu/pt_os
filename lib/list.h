/*
 * 双向循环链表
 * 
 * list_node应当放入需要的管理的new_struct内部
 * 在管理list_node时，new_struct自然的被串联起来
 *
 * author:Shuai Zhang (zhangshuaiisme@gmail.com)
 */

#ifndef LIST_H
#define LIST_H

#include "std_type_define.h"

/*	
 * 链表节点
 * 大小：8 byte
 */
struct list_node{
	struct list_node * prev;
	struct list_node * next;
};

/*
 * list所在结构的及地址
 * 用法：list_entry(&list_node,struct page,list_node)
 * */
#define list_entry(link, type, member) \
	((type *)((char *)(link)-(unsigned long)(&((type *)0)->member)))



void list_init(struct list_node *list_head);
bool list_empty(struct list_node *list_head);
void list_insert(struct list_node *list_head, struct list_node *new_node);
void list_remove(struct list_node *old_node);



#endif
