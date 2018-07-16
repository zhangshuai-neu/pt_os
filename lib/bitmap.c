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
 * 置位为1
 * bit_index: 要确认的
 * */
void bitmap_set_bit(struct bitmap *bm,uint32_t bit_index){
	uint32_t byte_index = bit_index/8;
	uint8_t bit_index_in_byte = bit_index % 8;
	
	//要判断的字节地址
	uint8_t * verify_byte_addr = bm->bits + byte_index;
	//生成要判断bit的"与字节"
	uint8_t or_byte = VERIFY_BIT(bit_index_in_byte);	
	
	*verify_byte_addr = *verify_byte_addr | or_byte;
}

/*
 * 清空位
 * bit_index: 要确认的
 * */
void bitmap_clear_bit(struct bitmap *bm,uint32_t bit_index){
	uint32_t byte_index = bit_index/8;
	uint8_t bit_index_in_byte = bit_index % 8;
	
	//要判断的字节地址
	uint8_t * verify_byte_addr = bm->bits + byte_index;
	//生成要判断bit的"与字节"
	uint8_t and_byte = ~(VERIFY_BIT(bit_index_in_byte));
	
	*verify_byte_addr = *verify_byte_addr & or_byte;
}

/*
 * apply some "continuous" bits in bitmap
 * return: allocated bit_index (if it's 0, which means application failure)
 * */
uint32_t bitmap_apply(struct bitmap *bm){
	

}


