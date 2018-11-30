#include "lock.h"

// 互斥锁=======================

//初始化
void mutex_lock_init(struct mutex_lock * ml){
    ml->holder = NULL;
    mutex_semaphore_init(&ml->ms);
}

void mutex_lock_get(struct mutex_lock * ml, struct task * holder){
    if( mutex_semaphore_get(&ml->ms) ){
        // 获取了信号量
        ml->holder = holder;
    } else {
        //获取失败
        list_insert(&(ml->ms.wait_list_head),&holder->block_link);

        // 将自己阻塞
    }

}


void mutex_lock_release(struct mutex_lock * ml, struct task * holder){
    
    
}
