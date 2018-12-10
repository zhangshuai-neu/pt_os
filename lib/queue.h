#ifndef QUEUE_H
#define QUEUE_H

#include "std_type_define.h"

struct queue{
    void * base_addr;   // queue 空间的基地址
    int unit_size;      // queue unit的大小
    int unit_num;       // queue unit的最大数量
    
    int unit_head;     // 记录头
    int unit_tail;     // 记录尾, 一直指向一个空元素
    
    int queue_send_block;      // 发送锁，1是未加锁，0加锁
    int queue_receive_block;   // 接收锁，1是未加锁，0加锁
};

#endif 

