/*
 * 磁盘接口 实现
 *
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "disk_interface.h"
#include "io.h"
#include "system_call.h"
#include "mm.h"

//全局结构
// 按硬盘数计算的通道数，只使用1个
uint8_t * channel_cnt = (uint8_t *)CHANNEL_COUNT_ADDR;	   		
// 有两个ide通道，只使用1个	
struct ide_channel * channel = (struct ide_channel *)CHANNEL_STRUCT_ADDR;		

/*
 * 从磁盘读取连续的block到指定内存
 *
 * mem_base_addr : 目的内存基地址（物理地址）
 * block_id : block起始号
 * block_num : block数量
 * 
 * */
void disk_read_block(void * mem_base_addr,uint32_t block_id, uint32_t block_num){
		
}

/*
 * 将内容写入到连续的 block
 * 
 * mem_base_addr : 源内存基地址（物理地址）
 * mem_length ： 读取的长度，根据长度
 * block_id : block起始号
 * 
 * */
void disk_write_block(void * mem_base_addr,uint32_t mem_length, uint32_t block_id){

}

/* 
 * 硬盘数据结构初始化 
 * 使用一个ide接口连接一个主硬盘
 * */
void ide_init() { 
	*channel_cnt = 1;	//通道数量，默认为1

	ptsc_memset(channel,0,sizeof(struct ide_channel));
	
	ptsc_strcpy(channel->name,"ide0");
	
	//为ide通道初始化端口基址及中断向量
	channel->port_base	 = 0x1f0;	   // ide0通道的起始端口号是0x1f0
	channel->irq_no	 = 0x20 + 14;	   // 从片8259a上倒数第二的中断引脚,也就是ide0通道的的中断向量号
		
	channel->expecting_intr = FALSE;   // 未向硬盘写入指令时不期待硬盘的中断
	
	return ;
}

