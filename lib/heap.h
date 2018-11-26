/*
实现动态的内存分配
按照heap的方式管理已经映射好的内存
*/

#ifndef HEAP_H
#define HEAP_H

#include "std_type_define.h"

struct heap{
    uint32_t heap_base_addr;         //heap空间的基地址
    uint32_t heap_end_addr;
    uint32_t heap_block_count;
};

struct heap_block{
    uint32_t block_base_addr;
    uint32_t block_size;
};

#endif
