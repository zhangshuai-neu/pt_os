/*
实现动态的内存分配
按照heap的方式管理已经映射好的内存
*/

#ifndef HEAP_H
#define HEAP_H

#include "std_type_define.h"

struct heap_block{
    struct heap_block * next_block_ptr;
    uint32_t block_size;
};

struct heap{
    // heap空间的基地址，同时也是heap_block头的位置
    struct heap_block* hb_head;
    struct heap_block* hb_end;
    // heap的大小   
    uint32_t heap_size;
};

#define UINT32_TOP_BIT ((uint32_t)1<<31)

#define SET_FREE(BLOCK_ADDR) ((uint32_t)BLOCK_ADDR | UINT32_TOP_BIT)
#define SET_USE(BLOCK_ADDR)  ((uint32_t)BLOCK_ADDR & (!UINT32_TOP_BIT)

#define USE_BLOCK  ((uint8_t)0)
#define FREE_BLOCK ((uint8_t)1)

#endif
