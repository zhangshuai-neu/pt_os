/*
    参考FreeRTOS heap4
    使用heap结构管理一段内存
    管理方式：
        管理heap_block的链表
        每个heap_block包含heap_block结构和申请的内存空间
        next_block_ptr的UINT32_TOP_BIT表明是不是
        heap_block的block_size：需要减去heap_block本身的size

*/

#include "heap.h"

// heap_block初始化
// flag 指明是free还是use
void heap_block_init(struct heap_block* hp, uint32_t size, uint8_t flag){
    hp->next_block_ptr = NULL;
    hp->block_size = size;
    if(flag==FREE_BLOCK){
        SET_FREE(hp);
    }
    if(flag==USE_BLOCK){
        SET_USE(hp);
    }
}

/*
// not be used

// 从小到大的顺序插入
// in_block的位置一定在 head和end之间
void heap_block_insert(struct heap_block* head, struct heap_block * in_block){
    struct heap_block* hb_temp = head;
    struct heap_block* pre = hb_temp;
    while((uint32_t)hb_temp < (uint32_t)in_block){
        pre = hb_temp;
        hb_temp = hb_temp->next_block_ptr;
    }
    pre->next_block_ptr = in_block;
    in_block->next_block_ptr = hb_temp;
}
*/

// heap初始化
void heap_init(struct heap * hp, uint32_t heap_base_addr, uint32_t heap_size){

    hp->heap_size = heap_size;

    // heap_block 头尾结点初始化 
    hp->hb_head = (struct heap_block*)heap_base_addr;
    
    uint32_t end_addr = heap_base_addr + heap_size -BLOCK_SZIE;
    hp->hb_end = (struct heap_block *)end_addr;

    heap_block_init(hp->hb_head,0,USE_BLOCK);
    heap_block_init(hp->hb_end,0,USE_BLOCK);
    hp->hb_head->next_block_ptr = hp->hb_end;

    // 初始化 free_block, 并将其插入链表
    struct heap_block * free_block = (hp->hb_head + 1);
    // 只减去use的block
    heap_block_init(free_block, heap_size - 3*BLOCK_SZIE, FREE_BLOCK);
    
    // 链接
    hp->hb_head->next_block_ptr = free_block;
    free_block->next_block_ptr = hp->hb_end;
    hp->hb_end->next_block_ptr = NULL;

    // heap剩余的空间大小
    hp->heap_left_size = heap_size - 3*BLOCK_SZIE;
}

/**
 * 从heap中申请一段内存
 * 找到大小最合适的free_block，
 * 如果free_block比较大，则将其分割
 **/ 
void * heap_malloc(struct heap *hp, uint32_t size){
    // 申请的空间大小，需要包括一个heap_block
    size = size + BLOCK_SZIE;
    if (hp->heap_left_size < size){
        //空间不足
        return NULL;
    }

    // 遍历所有的free_block，找到合适free_block
    struct heap_block * hb_temp = hp->hb_head->next_block_ptr;
    struct heap_block * alloc_block = NULL;
    while(hb_temp != hp->hb_end){
        if(IS_FREE(hb_temp)){
            // 当前block的size足够大
            if(hb_temp->block_size >= size){
                if(!alloc_block)
                    // alloc_block为空
                    alloc_block = hb_temp;
                else{
                    // alloc_block不为空
                    // block的size尽量小
                    if(alloc_block->block_size >= hb_temp->block_size){
                        alloc_block = hb_temp;
                    }
                }
            }
        }
        hb_temp = hb_temp->next_block_ptr;
    }

    // 对分配的free_block进行分割
    // 如果剩余的空间比struct heap_block的size要大才进行分割
    if(alloc_block->block_size - size > BLOCK_SZIE){
        struct heap_block * new_free_block = (struct heap_block *)((uint32_t)alloc_block + size);
        new_free_block->block_size = alloc_block->block_size - size - BLOCK_SZIE;
        SET_FREE(new_free_block);

        // alloc_block修改
        alloc_block->block_size = size - BLOCK_SZIE; 
        SET_USE(alloc_block);
        
        // 链接
        new_free_block->next_block_ptr = alloc_block->next_block_ptr;
        alloc_block->next_block_ptr = new_free_block;
        
    }

    // 剩余空间
    hp->heap_left_size -= (size + BLOCK_SZIE);


    // 确定返回的地址
    uint32_t ret_addr = (uint32_t)alloc_block;
    ret_addr = ret_addr + BLOCK_SZIE;
    return (void*)ret_addr;
}


//归还heap中的内存
void heap_free(struct heap *hp, uint32_t block_base_addr){
    uint32_t free_space_size = 0;

    // 找到block结构位置，并设为free状态
    struct heap_block * do_free_block = (struct heap_block *)(block_base_addr - BLOCK_SZIE);
    SET_FREE(do_free_block);
    free_space_size = do_free_block->block_size;

    // 是否能与 next block 合并
    if(IS_FREE(do_free_block->next_block_ptr)){
        // 可以合并
        do_free_block->block_size += (BLOCK_SZIE + do_free_block->next_block_ptr->block_size);
        do_free_block->next_block_ptr = do_free_block->next_block_ptr->next_block_ptr;

        free_space_size += BLOCK_SZIE;
    }

    // 是否能与 pre_block 合并
    struct heap_block * hb_pre = hp->hb_head;
    while(hb_pre!=NULL && hb_pre->next_block_ptr != do_free_block)
        hb_pre = hb_pre->next_block_ptr;
    
    if(IS_FREE(hb_pre)){
        // 可以合并
        hb_pre->next_block_ptr = do_free_block->next_block_ptr;
        hb_pre->block_size += (do_free_block->block_size + BLOCK_SZIE);

        free_space_size += BLOCK_SZIE;
    }

    hp->heap_left_size +=  free_space_size;
}


