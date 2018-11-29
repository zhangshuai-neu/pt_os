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

//=====================
// 二值信号量
// 用来进行同步，初始为0
//=====================
enum bin_value {
    ZERO, ONE
};
struct bin_semaphore{
    enum bin_value value;
    uint8_t holder_count;
};

//=====================
// 计数信号量
// 用来进行资源管理，初始为资源数量
//=====================
struct count_semaphore{
    uint32_t count;
    uint8_t holder_count;
};

//=====================
// 互斥量
// 用来进行互斥访问,初始为1
//=====================
struct mutex_semaphore{
    enum bin_value mutex_value;
    uint8_t holder_count;
}


#endif
