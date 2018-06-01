/*
 * 双向循环链表
 * 
 * list_node应当放入需要的管理的new_struct内部
 * 在管理list_node时，new_struct自然的被串联起来
 *
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
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
 * 可以用来获取list所在结构的基地址，和其他结构的地址
 * 用法：list_entry(&list_node,struct page,list_node)
 * */
#define list_entry(link, type, member) \
	((type *)((char *)(link)-(unsigned long)(&((type *)0)->member)))

/*
 * list的尾部节点
 * 用法：list_tail(&list_head)
 * */
#define list_tail(list_head) \
	(list_head->prev)

void list_init(struct list_node *list_head);
void list_insert(struct list_node *list_head, struct list_node *new_node);
void list_remove(struct list_node *old_node);



#endif
