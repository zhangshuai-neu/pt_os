/*
 * 链表
 * 1）双向循环链表
 * 2）单链表
 *
 author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "std_type_define.h"
#include "list.h"

//======================================================================
//1 双向循环链表
//======================================================================
//初始化
void list_init(struct list_node *list_head)
{
	list_head->next = list_head;
	list_head->prev = list_head;
}

//在list_head之前插入新节点
void list_insert(struct list_node *list_head, struct list_node *new_node)
{
	if(list_head==NULL || new_node==NULL) return ;
	//新节点插入头部之前
	new_node->prev		= list_head->prev;
	new_node->next		= list_head;
	//尾节点指向新节点
	new_node->prev->next	= new_node;
	new_node->next->prev	= new_node;
}

//将old_node从其所在链表删除
//如果只有一个，该节点自己就一个新的list
//被删除的节点，在list_init之后也变成一个新的list
void list_remove(struct list_node *old_node)
{
	if(old_node == NULL) return ;
		
	//有多个节点
	old_node->prev->next = old_node->next;
	old_node->next->prev = old_node->prev;
}

//======================================================================
//2 单链表
//======================================================================






