/*
    参考FreeRTOS heap4
    使用heap结构管理一段内存
    管理方式：
        管理heap_block的链表
        每个heap_block包含heap_block结构和申请的内存空间
        next_block_ptr的UINT32_TOP_BIT表明是不是

*/

#include "heap.h"

// heap_block初始化
// flag 指明是free还是use
void heap_block_init(struct heap_block* hp, uint32_t size, uint8_t flag){
    hp->next_block_ptr = NULL;
    hp->block_size = size;
    if(flag==FREE_BLOCK){
        SET_FREE(hp->next_block_ptr);
    }
    if(flag==USE_BLOCK){
        SET_USE(hp->next_block_ptr);
    }
}

// 从小到大的顺序插入
// in_block的位置一定在 head和end之间
void heap_block_insert(struct heap_block* head, struct heap_block * in_block){
    struct heap_block* temp = head;
    struct heap_block* pre = temp;
    while((uint32_t)temp < (uint32_t)in_block){
        pre = temp;
        temp = temp->next_block_ptr;
    }
    pre->next_block_ptr = in_block;
    in_block->next_block_ptr = temp;
}


// heap初始化
void heap_init(struct heap * hp, uint32_t heap_base_addr, uint32_t heap_size){

    hp->heap_size = heap_size;

    // heap_block 头尾结点初始化 
    hp->hb_head = (struct heap_block*)heap_base_addr;
    
    uint32_t end_addr = heap_base_addr + heap_size -sizeof(struct heap_block);
    hp->hb_end = (struct heap_block *)end_addr;

    heap_block_init(hp->hb_head,0,FREE_BLOCK);
    heap_block_init(hp->hb_end,0,USE_BLOCK);
    hp->hb_head->next_block_ptr = hp->hb_end;

    // 初始化 free_block, 并将其插入链表
    struct heap_block * free_block = (hp->hb_head + 1);
    heap_block_init(free_block, heap_size - 3*sizeof(struct heap_block));
    heap_block_insert(hp->hb_head, free_block);
}

/**
 * 从heap中申请一段内存
 * 找到大小最合适的free_block，
 * 如果free_block比较大，则将其分割

 **/ 
void * heap_malloc(struct heap *hp, uint32_t size){
    
}

//归还heap中的内存
void heap_free(struct heap *hp, uint32_t block_base_addr){

}
