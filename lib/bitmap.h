/*
 * 位示图
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */
#ifndef BITMAP_H
#define BITMAP_H

#include "std_type_define.h"、

//----------------宏定义---------------
#define VERIFY_BIT(index_in_byte) ((uint8_t)1 << index_in_byte)

//----------------结构-----------------
struct bitmap {
   uint32_t btmp_bytes_len;
	/* 在遍历位图时,整体上以字节为单位,细节上是以位为单位,所以此处位图的指针必须是单字节 */
   uint8_t* bits;
};

//----------------函数声明--------------
void init_bitmap(struct bitmap *bm);

#endif
