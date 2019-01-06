#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../semaphore.h"

void print_bool(bool in){
    if(in==TRUE){
        printf("TRUE\n");
    } else {
        printf("FLASE\n");
    }
}

int main(){
    bin_semaphore * bs_ptr = (bin_semaphore*)malloc(sizeof(bin_semaphore));

    // 初始化
    bin_semaphore_init(bs_ptr);
    // 申请一次，return true
    print_bool(bin_semaphore_get(bs_ptr));
    // 申请一次，因为已经申请过，所以 return false
    print_bool(bin_semaphore_get(bs_ptr));
    // 释放一次
    bin_semaphore_release(bs_ptr);
    // 申请一次，因为已经释放，所以 return true
    print_bool(bin_semaphore_get(bs_ptr));

    return 0;
}