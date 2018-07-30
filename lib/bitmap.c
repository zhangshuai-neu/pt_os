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
	for(uint32_t i=0; i<bm->btmp_bytes_len; i++){
		*(bm->bits + i) = 0;
	}
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
void bitmap_set_cont_bits(struct bitmap *bm, uint32_t start_bit_index,  \
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
 * 判断该byte有几个可用字节
 * 从高位开始，到1为止
 */
uint8_t verify_valid_bit_in_byte(uint8_t byte){
	uint8_t count=0;
	uint8_t diff_byte = ((uint8_t)1)<<7;
	while(count<8 && (diff_byte & byte) == 0){
		diff_byte=diff_byte >> 1;
		count++;
	}
	return count;
}

//判断申请位置是否合理，由bitmap_apply调用
bool is_valid_allocation(uint8_t * byte_headr, uint32_t start_bit_index ,	\
	uint32_t apply_bits_num){

	uint32_t count=0;
	uint32_t remain_bits_num;

	//判断起始字节有几个可用的bit
	uint32_t start_byte_index = start_bit_index / 8;
	uint8_t count = verify_valid_bit_in_byte(byte_headr[start_byte_index]);
	if( count != (start_byte_index+1)*8-start_bit_index){
		//start_bit_index 靠前,例如0010 1111 的第5位（从低到高）
		return FALSE;
	}
	if(apply_bits_num < count){
		//在 start_byte_index 完成申请
		return TRUE;
	}else{
		//在 start_byte_index 申请几个bits
		remain_bits_num = apply_bits_num - count;
		count = 0;
	}

	//判断字节是否可用
	int remain_byte = remain_bits_num/8;
	for(uint32_t i=0; i<remain_byte; i++){
		if(!ALL_SET0(byte_headr[(start_bit_index + i+1)])){
			//后续字节存在置1的情况
			return FALSE;
		}
	}

	//并未发生上述错误 ok
	return TRUE;
}

/*
 * apply some "continuous" bits in bitmap
 * 申请连续的bit，成功则被置1，并返回相应起始索引
 * 1）先用暴力方法来处理吧
 * base_bit_index: 从此处开始扫描，之前的不考虑
 * apply_bits_num: 申请的bit数
 * return: allocated bit_index (if it's 0, which means application failure)
 * */
uint32_t bitmap_alloc_cont_bits(struct bitmap *bm, uint32_t base_bit_index, uint32_t apply_bits_num){
	for(uint32_t i=base_bit_index; i<BITMAP_BYTE_SIZE*8; i++){
		if(bitmap_verify_bitset(bm,i)){
			//i指向bit为0
			if(is_valid_allocation(bm->bits,i,apply_bits_num)){
				//判断是否为合法起始bit_index
				bitmap_set_cont_bits(bm,i,apply_bits_num,1);
				return i;
			}
			else{
				return 0;
			}
		}
	}
	return 0;	//第几个资源
}

//回收一些连续bits
void bitmap_recycle_cont_bits(struct bitmap *bm, uint32_t start_bit_index,  \
	int32_t bit_length/* from 1*/){

	bitmap_set_cont_bits(bm,start_bit_index,bit_length,0);
}
