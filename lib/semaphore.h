#ifndef LOCK_H
#define LOCK_H

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
