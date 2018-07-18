/*
 * 位示图
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */
#ifndef BITMAP_H
#define BITMAP_H

#include "std_type_define.h"、

//----------------宏定义---------------
#define VERIFY_BIT(index_in_byte) ((uint8_t)1 << index_in_byte)

#define ALLOC_BIT_BEGIN_INDEX ((uint32_t)2048)  //用户分配的起始地址

//----------------结构-----------------
struct bitmap {
   uint32_t btmp_bytes_len;
	/* 在遍历位图时,整体上以字节为单位,细节上是以位为单位,所以此处位图的指针必须是单字节 */
   uint8_t* bits;
};

//----------------函数声明--------------
void init_bitmap(struct bitmap *bm);
bool bitmap_verify_bitset(struct bitmap *bm,uint32_t bit_index);
void bitmap_set_bit(struct bitmap *bm,uint32_t bit_index, uint8_t mask/* 1 or 0*/);
void bitmap_set_cont_bit(struct bitmap *bm, uint32_t start_bit_index,  \
	int32_t bit_length/* from 1*/ ,uint8_t mask/* 1 or 0*/);

#endif
