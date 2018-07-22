/*
 * 位示图
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */
#ifndef BITMAP_H
#define BITMAP_H

#include "std_type_define.h"、

//----------------宏定义---------------
//生成指定位为1的byte数据
#define VERIFY_BIT(index_in_byte) ((uint8_t)1 << index_in_byte)
//判断各位是否都为1
#define ALL_SET1(judge_byte)  (judge_byte == (uint8_t)0xff)
//判断是否全为0
#define ALL_SET0(judge_byte)  (judge_byte == (uint8_t)0x00)
//bitmap size
#define BITMAP_BYTE_SIZE  SIZE_4K

//----------------结构-----------------
struct bitmap {
  //以字节为单位进行遍历
  uint32_t btmp_bytes_len;
  uint8_t* bits;
};

//----------------函数声明--------------
//初始化所有bits为0
void init_bitmap(struct bitmap *bm);

//确认bit是否置1
bool bitmap_verify_bitset(struct bitmap *bm,uint32_t bit_index);

//将某一bit置位，值由mask决定
void bitmap_set_bit(struct bitmap *bm,uint32_t bit_index, uint8_t mask/* 1 or 0*/);

//将连续的bits置位，值由mask决定
void bitmap_set_cont_bits(struct bitmap *bm, uint32_t start_bit_index,  \
	int32_t bit_length/* from 1*/ ,uint8_t mask/* 1 or 0*/);

//从bitmap中申请一些连续bit使用
uint32_t bitmap_alloc_cont_bits(struct bitmap *bm, uint32_t base_bit_index, \
  uint32_t apply_bits_num)

//回收一些连续bits
void bitmap_recycle_cont_bits(struct bitmap *bm, uint32_t start_bit_index,  \
	int32_t bit_length/* from 1*/);

#endif
