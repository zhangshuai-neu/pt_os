/*
实现动态的内存分配
按照heap的方式管理已经映射好的内存
*/

#ifndef HEAP_H
#define HEAP_H

#include "std_type_define.h"

struct heap_block{
    // 记录下一个block的位置
    struct heap_block * next_block_ptr;
    // block的内存大小，同时最高位的1bit用来标记该block是否free，省一个byte
    uint32_t block_size;
};

struct heap{
    // heap空间的基地址，同时也是heap_block头的位置
    struct heap_block* hb_head;
    struct heap_block* hb_end;
    // heap的大小   
    uint32_t heap_size;
    // 剩余空间
    uint32_t heap_left_size;
};

#define BLOCK_SZIE  (sizeof(struct heap_block))

// 32位数据的最高bit
#define UINT32_TOP_BIT ((uint32_t)1<<31)

// block是否free
// free返回1，否则为0
#define IS_FREE(BLOCK_PTR)  !(BLOCK_PTR->block_size & UINT32_TOP_BIT)

// block设为free或者use
// use置1，free置0
// free: 置0时size可以直接参数size的比较(很方便)
// use: 置1时，因为size不需要参与判断，所以block_size被修改也无所谓
#define SET_USE(BLOCK_PTR)   (BLOCK_PTR->block_size = BLOCK_PTR->block_size | UINT32_TOP_BIT) 
#define SET_FREE(BLOCK_PTR)  (BLOCK_PTR->block_size = BLOCK_PTR->block_size & (!UINT32_TOP_BIT))

#define USE_BLOCK  ((uint8_t)1)
#define FREE_BLOCK ((uint8_t)0)

//==============================
// 函数声明
//==============================

void heap_init(struct heap * hp, uint32_t heap_base_addr, uint32_t heap_size);
void * heap_malloc(struct heap *hp, uint32_t size);
void heap_free(struct heap *hp, uint32_t block_base_addr);

#endif
