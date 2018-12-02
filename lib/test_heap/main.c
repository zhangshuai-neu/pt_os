#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../heap.h"

void heap_block_show(struct heap_block *hb){
    
}

void heap_show(struct heap * hp){
    struct heap_block * hb_temp;
    hb_temp = hp->hb_head;
    while(hb_temp != hp->hb_end){
        

    }
}

int main(){
    void * heap_mem = malloc(SIZE_1K);

    //测试heap的功能
    struct heap hp;
    // 初始化
    heap_init(&hp,(uint32_t)heap_mem,SIZE_1K);

    // 申请内存
    uint32_t * ui1 = (uint32_t*)heap_malloc(&hp,16);
    uint32_t * ui2 = (uint32_t*)heap_malloc(&hp,16);
    uint32_t * ui3 = (uint32_t*)heap_malloc(&hp,16);
    uint32_t * ui4 = (uint32_t*)heap_malloc(&hp,16);

    // 回收内存
    heap_free(&hp,(uint32_t)ui2);
    heap_free(&hp,(uint32_t)ui3);

    free(heap_mem);
}
