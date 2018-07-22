//暂时不需要，之后在处理

#include "queue.h"

//---------------------uint_key 为标准进行实现----------------------------------
void init_queue_node(queue_node *node, uint8_t key_type/*指明key类型*/){
  switch(key_type){
    case 1:
      (node->key).uint_key = 0;
      break;
    default:
      (node->key).uint_key = 0;
  }
  node->next = NULL;
}

//以链表的方式进行管理
bool link_queue_insert(queue_node *queue_head, queue_node * new_node){
  if(queue_head == NULL || new_node=NULL){
    //错误处理
    return FALSE;
  }


}
bool link_queue_remove(){

}

//以堆方式进行管理
