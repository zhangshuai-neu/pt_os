#include "semaphore.h"

//=====================
// 二值信号量
//=====================

void bin_semaphore_init(struct bin_semaphore * bs_ptr){
    bs_ptr->value = ZERO;
    bs_ptr->holder_count = 0;
}

bool bin_semaphore_get(struct bin_semaphore * bs_ptr){
    if(bs_ptr->value == ZERO){
        return FALSE;
    }else{
        bs_ptr->value = ZERO;
        return TRUE;
    }
}

void bin_semaphore_release(struct bin_semaphore * bs_ptr){
    bs_ptr->value = ONE;
}

//=====================
// 计数信号量
// 用来进行资源管理，初始为资源数量
//=====================
void count_semaphore_init(struct count_semaphore *cs_ptr, uint32_t resource_num){
    cs_ptr->count = resource_num;
    cs_ptr->holder_count = 0;
}

bool count_semaphore_get(struct count_semaphore *cs_ptr){
    if(cs_ptr->count == 0){
        return FALSE;
    }else{
        (cs_ptr->count)--;
        return TRUE;
    }
}

void count_semaphore_release(struct bin_semaphore * bs_ptr){
    (bs_ptr->value)++;
}

//=====================
// 互斥量
// 用来进行互斥访问,初始为1,资源数量为1
//=====================
void mutex_semaphore_init(struct mutex_semaphore * ms_ptr){
    ms_ptr->mutex_value  = ONE;
    ms_ptr->holder_count = 0;
}

bool mutex_semaphore_get(struct mutex_semaphore * ms_ptr){
    if(ms_ptr->mutex_value == ZERO){
        return FALSE;
    }else{
        ms_ptr->mutex_value = ZERO;
        return TRUE;
    }
}

void mutex_semaphore_release(struct mutex_semaphore * ms_ptr){
    ms_ptr->mutex_value = ONE;
}