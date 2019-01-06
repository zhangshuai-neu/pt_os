#include "lock.h"

// 互斥锁=======================

//初始化
void mutex_lock_init(struct mutex_lock * ml){
    ml->holder = NULL;
    mutex_semaphore_init(&ml->ms);
}

void mutex_lock_get(struct mutex_lock * ml, struct task * holder){
    //进入临界区，禁用中断
    intr_disable();

    struct list_node * wait_list_head = &( (ml->ms).wait_list_head );
    while ( !(ml->holder!=NULL && mutex_semaphore_get(&ml->ms) ) ){
        //获取锁失败, 阻塞当前任务
        thread_block(wait_list_head);
    }
    //获取锁成功
    ml->holder = holder;

    //离开临界区，启用中断
    intr_enable();
}


void mutex_lock_release(struct mutex_lock * ml, struct task * holder){
    //进入临界区，禁用中断
    intr_disable();
    
    //离开临界区，启用中断
    intr_enable();
}
