/*
 * 磁盘接口 头文件
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef DISK_INTERFACE
#define DISK_INTERFACE

#include "std_type_define.h"
#include "disk_info.h"

//--------------------------------------结构定义-----------------------------------------------------

/* ata通道结构 
 * size: 8 + 2 + 1 + 4 + (8+4+1) = 28 byte < 32 byte
 */
struct ide_channel {
	char name[8];		 	// 本ata通道名称, 如ata0,也被叫做ide0. 可以参考bochs配置文件中关于硬盘的配置。
	uint16_t port_base;		// 本通道的起始端口号
	uint8_t irq_no;		 	// 本通道所用的中断号
   	bool expecting_intr;		// 向硬盘发完命令后等待来自硬盘的中断
	struct disk devices;	 	// 一个通道上连接两个硬盘，一主一从
};

/* 硬盘结构 */
struct disk {
	char name[8];			   		   // 本硬盘的名称，如sda等
	struct ide_channel* my_channel;	   // 此块硬盘归属于哪个ide通道
	uint8_t dev_no;			   		   // 本硬盘是主0还是从1
};


//--------------------------------------全局宏定义---------------------------------------------------

#define SIZE_4M ((uint32_t)1024*1024*4)
#define CHANNEL_COUNT_ADDR 	SIZE_4M		//channel_cnt 只使用一个字节
#define CHANNEL_STRUCT_ADDR	(CHANNEL_COUNT_ADDR+1)	//channel 结构使用32byte(前28个)


//--------------------------------------函数声明-----------------------------------------------------

//初始化ide接口和磁盘
void ide_init();

// 从磁盘读取数据到指定内存
void disk_read_block(void * mem_base_addr,uint32_t block_id, uint32_t block_num);

// 将内容写入到连续的block, 使用者自己计算如何使用
void disk_write_block(void * mem_base_addr,uint32_t mem_length, uint32_t block_id);

#endif
