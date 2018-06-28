#include <stdio.h>
#include "../list.h"

//使用list将int链表呈现出一种类似面向对象的感觉
typedef struct{
	int var;
	struct list_node link;
}int_vector;

//创建一个int list节点
int_vector* get_int_vector(int var){
	int_vector* ret=malloc(sizeof(int_vector));
	ret->var=var;
	list_init(&(ret->link));
	return ret;
}
void show_int_vector(int_vector *vec_head){
	printf("\n");
	int_vector end;
	list_insert(&(vec_head->link),&(end.link));
	printf("var_list: ");
	while(&(vec_head->link) != &(end.link)){
		printf("%d ",vec_head->var);
		vec_head=list_entry(vec_head->link.next,int_vector,link);
	}
	printf("\n");
}

int main(){
	int index=0;
	int_vector* vec_head=get_int_vector(-1);
	int_vector* new_int_vec=NULL;
	for(index=0;index<10;index++){
		new_int_vec=get_int_vector(rand());
		printf("new_int: %d\n",new_int_vec->var);
		list_insert(&(vec_head->link),&(new_int_vec->link));
	}
	show_int_vector(vec_head);
}
