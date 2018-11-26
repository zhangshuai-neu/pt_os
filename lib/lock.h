#include "std_type_define.h"
#include "tm.h"
#include "list"

#ifndef LOCK_H
#define LOCK_H

//=====================
// 二值信号量
//=====================
enum bin_value {
    ZERO, ONE
};
struct bin_semaphore{
    struct task * holder;
    enum bin_value value;
    uint8_t holder_count;
};

//=====================
// 计数信号量
//=====================
struct count_semaphore{
    struct task * holder;
    uint32_t count;
    uint8_t holder_count;
};

//=====================
// 互斥量
//=====================


#endif
