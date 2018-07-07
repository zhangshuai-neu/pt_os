/*
 * 磁盘接口 实现
 *
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "disk_interface.h"
#include "io.h"
#include "sc.h"

//全局结构
uint8_t * channel_cnt = CHANNEL_COUNT_ADDR;	   			// 按硬盘数计算的通道数，只使用1个
struct ide_channel * channel = CHANNEL_STRUCT_ADDR;		// 有两个ide通道，只使用1个

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
	struct ide_channel* channel = hd->my_channel;

   /* 写入要读写的扇区数*/
   outb(reg_sect_cnt(channel), sec_cnt);	 // 如果sec_cnt为0,则表示写入256个扇区

   /* 写入lba地址(即扇区号) */
   outb(reg_lba_l(channel), lba);		 	 // lba地址的低8位,不用单独取出低8位.outb函数中的汇编指令outb %b0, %w1会只用al。
   outb(reg_lba_m(channel), lba >> 8);		 // lba地址的8~15位
   outb(reg_lba_h(channel), lba >> 16);		 // lba地址的16~23位

   /* 因为lba地址的24~27位要存储在device寄存器的0～3位,
    * 无法单独写入这4位,所以在此处把device寄存器再重新写入一次*/
   outb(reg_dev(channel), BIT_DEV_MBS | BIT_DEV_LBA | (hd->dev_no == 1 ? BIT_DEV_DEV : 0) | lba >> 24);

}


/* 
 * 硬盘数据结构初始化 
 * 使用一个ide接口连接一个主硬盘
 * */
void ide_init() { 
	*channel_cnt = 1	//通道数量，默认为1

	ptsc_memset(channel,0,sizeof(struct ide_channel));
	ptsc_strcpy(channel->name,"ide0");
	
	//为ide通道初始化端口基址及中断向量
	channel->port_base	 = 0x1f0;	   // ide0通道的起始端口号是0x1f0
	channel->irq_no	 = 0x20 + 14;	   // 从片8259a上倒数第二的中断引脚,也就是ide0通道的的中断向量号
		
	channel->expecting_intr = false;		   // 未向硬盘写入指令时不期待硬盘的中断
}

