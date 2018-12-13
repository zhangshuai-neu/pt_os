#ifndef QUEUE_H
#define QUEUE_H

#include "std_type_define.h"

// 实现一个循环队列
struct queue{
    void * base_addr;   // queue 空间的基地址
    int unit_size;      // queue unit的大小
    int unit_num;       // queue unit的最大数量
    
    int unit_head;     // 记录头
    int unit_tail;     // 记录尾, 一直指向一个空元素
    
    char queue_send_block;      // 发送锁，1是未加锁，0加锁
    char queue_receive_block;   // 接收锁，1是未加锁，0加锁
};

// 只能添加unit_num-1个unit，因为用一个空的表示unit满了
bool queue_init(struct queue * q, void* base_addr, int unit_size, int unit_num);
bool queue_is_empty(struct queue * q);
bool queue_is_full(struct queue * q);
bool queue_send_value(struct queue * q, void * unit_ptr);
bool queue_receive_value(struct queue * q, void * unit_ptr);

#endif 

