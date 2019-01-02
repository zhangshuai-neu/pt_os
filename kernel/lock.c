#include "lock.h"

// 互斥锁=======================

//初始化
void mutex_lock_init(struct mutex_lock * ml){
    ml->wait_list_head = NULL;
    mutex_semaphore_init(&ml->ms);
}

void mutex_lock_get(struct mutex_lock * ml, struct task * holder){
    
}


void mutex_lock_release(struct mutex_lock * ml, struct task * holder){
    
    
}
