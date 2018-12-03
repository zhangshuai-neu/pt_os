#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../heap.h"

void heap_block_show(struct heap_block *hb){
    printf("hb_addr: %x\n",(uint32_t)hb);
    printf("hb next_block_ptr: %x\n",(uint32_t)(hb->next_block_ptr));
    printf("hb block_size: %x\n\n",(uint32_t)hb->block_size);
}

void heap_show(struct heap * hp){
    printf("16进制输出=========================\n");
    printf("hb_head: %x\n",hp->hb_head);
    printf("hb_end: %x\n",hp->hb_end);
    printf("heap_size: %x\n",hp->heap_size);
    printf("heap_left_size: %x\n\n",hp->heap_left_size);

    struct heap_block * hb_temp;
    hb_temp = hp->hb_head;
    while(hb_temp != NULL){
        heap_block_show(hb_temp);
        hb_temp = hb_temp->next_block_ptr;
    }
}

int main(){
    void * heap_mem = malloc(SIZE_1K);
   
    //测试heap的功能
    struct heap hp;
    // 初始化
    
    heap_init(&hp,(uint32_t)heap_mem,SIZE_1K);
    heap_show(&hp);

    // 申请内存
    
    uint32_t * ui1 = (uint32_t*)heap_malloc(&hp,16);
    uint32_t * ui2 = (uint32_t*)heap_malloc(&hp,16);
    uint32_t * ui3 = (uint32_t*)heap_malloc(&hp,16);
    uint32_t * ui4 = (uint32_t*)heap_malloc(&hp,16);
    heap_show(&hp);

    // 回收内存
    heap_free(&hp,(uint32_t)ui2);
    heap_free(&hp,(uint32_t)ui3);
    heap_show(&hp);

    free(heap_mem);
}
