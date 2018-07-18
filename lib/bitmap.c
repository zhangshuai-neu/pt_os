/*
 * 位示图
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "bitmap.h"
#include "std_type_define.h"
#include "system_call.h"

/*
 * 初始化bitmap
 * */
void init_bitmap(struct bitmap *bm){
	ptsc_memset(bm->bits,0,bm->btmp_bytes_len);
	return ;
}

/*
 * 确定某一位是否为1
 * bit_index: 要确认的
 * */
bool bitmap_verify_bitset(struct bitmap *bm,uint32_t bit_index){
	uint32_t byte_index = bit_index/8;
	uint8_t bit_index_in_byte = bit_index % 8;

	//要判断的字节地址
	uint8_t * verify_byte_addr = bm->bits + byte_index;

	//生成要判断bit的"与字节"
	uint8_t and_byte = VERIFY_BIT(bit_index_in_byte);
	return (and_byte & *verify_byte_addr);
}


/*
 * 将某一位置位为 1 or 0 (根据mask)
 * bit_index: 要确认的
 * */
void bitmap_set_bit(struct bitmap *bm,uint32_t bit_index, uint8_t mask/* 1 or 0*/){
	uint32_t byte_index = bit_index/8;
	uint8_t bit_index_in_byte = bit_index % 8;

	//要设置的字节地址
	uint8_t * set_byte_addr = bm->bits + byte_index;
	//生成要判断bit的"与字节"
	uint8_t op_byte = VERIFY_BIT(bit_index_in_byte);
	if(mask == 1)
		*set_byte_addr = *set_byte_addr | op_byte;
	else
		*set_byte_addr = *set_byte_addr & ~op_byte;
}

/*
 * 将连续位置位为 1 or 0 (根据mask)
 * start_bit_index: 要确认的
 * */
void bitmap_set_cont_bit(struct bitmap *bm, uint32_t start_bit_index,  \
	int32_t bit_length/* from 1*/ ,uint8_t mask/* 1 or 0*/){

	if(start_bit_index + bit_length > SIZE_4K*8){
		//错误，超出bitmap
		return ;
	}

	for(uint32_t i=0; i<bit_length; i++){
		bitmap_set_bit(bm,start_bit_index+i,mask);
	}
}

/*
 * apply some "continuous" bits in bitmap
 * 1）先用暴力方法来处理吧（从用户区域开始分配）
 * return: allocated bit_index (if it's 0, which means application failure)
 * */
uint32_t bitmap_apply(struct bitmap *bm,uint32_t apply_size){
	uint32_t alloc_bit_index=0;



	return alloc_bit_index;
}
