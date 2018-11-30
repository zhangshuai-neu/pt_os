/**
 * 信号量
 * 
 * 信号量的实现，并没有具体要求
 * freeRTOS是利用队列实现的，队列的元素表示了资源数量
 * 我直接使用一个变量来进行记录了
 * 
*/

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "std_type_define.h"
#include "list.h"


enum bin_value {
    ZERO, ONE
};

//=====================
// 二值信号量
// 用来进行同步，初始为0
//=====================
struct bin_semaphore{
    enum bin_value value;
    struct list_node wait_list_head;
};

//=====================
// 计数信号量
// 用来进行资源管理，初始为资源数量
//=====================
struct count_semaphore{
    uint32_t count;
    struct list_node wait_list_head;
};

//=====================
// 互斥量
// 用来进行互斥访问,初始为1
//=====================
struct mutex_semaphore{
    enum bin_value mutex_value;
    struct list_node wait_list_head;
};


//函数声明==============================
void bin_semaphore_init(struct bin_semaphore * bs_ptr);
bool bin_semaphore_get(struct bin_semaphore * bs_ptr);
void bin_semaphore_release(struct bin_semaphore * bs_ptr);

void count_semaphore_init(struct count_semaphore *cs_ptr, uint32_t resource_num);
bool count_semaphore_get(struct count_semaphore *cs_ptr);
void count_semaphore_release(struct bin_semaphore * bs_ptr);

void mutex_semaphore_init(struct mutex_semaphore * ms_ptr);
bool mutex_semaphore_get(struct mutex_semaphore * ms_ptr);
void mutex_semaphore_release(struct mutex_semaphore * ms_ptr);

#endif
