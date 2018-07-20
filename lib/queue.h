#ifndef QUEUE_H
#define QUEUE_H

#include "std_type_define.h"

//优先级队列
typedef struct queue_node{
  union key{
    uint32_t uint_key;  // type 1

    //may have others' key in the future 类似面向对象的父类 :)
  }
  strcut queue_node * next;
} queue_node;

struct queue{
  queue_node * head;
  queue_node * tail;
}




#endif
