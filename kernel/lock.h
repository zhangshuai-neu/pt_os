#ifndef LOCK_H
#define LOCK_H

#include "../lib/semaphore.h"
#include "../lib/list.h"
#include "tm.h"

// 互斥锁
struct mutex_lock{
    struct mutex_semaphore ms;
    struct task * holder;
};

#endif
