#include "heap.h"

/*
使用heap结构管理一段内存
管理方式：
heap内存的开头为heap_block，用来记录内存的使用情况
内存从heap内存的尾部开始分配

/-----
/heap_block
/v
/
/^
/allocte mem
/------

*/

// heap结构初始化
void init_heap(struct heap * hp, uint32_t base_addr, uint32_t end_addr){
    hp->heap_base_addr = base_addr;
    hp->heap_base_addr = end_addr;
    hp->heap_block_count = 0;
}

//从heap中申请一段内存
void * heap_malloc(struct heap *hp, uint32_t size){
    uint32_t index  = hp->heap_block_count+1
}

//归还heap中的内存
void heap_free(struct heap *hp, uint32_t block_base_addr){

}
