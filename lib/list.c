/*
 * 双向循环链表
 *
 author:Shuai Zhang (zhangshuaiisme@gmail.com)
 */

#include "std_type_define.h"
#include "list.h"

//初始化
void list_init(struct list_node *list_head)
{
	list_head->next = list_head;
	list_head->prev = list_head;
}

//在list_head之前插入新节点
void list_insert(struct list_node *list_head, struct list_node *new_node)
{
	//新节点插入头部之前
	new_node->prev		= list_head->prev;
	new_node->next		= list_head;
	//尾节点指向新节点
	new_node->prev->next	= new_node;
	new_node->next->prev	= new_node;
}

//将old_node从其所在链表删除
void list_remove(struct list_node *old_node)
{
	old_node->prev->next = old_node->next;
	old_node->next->prev = old_node->prev;
}
