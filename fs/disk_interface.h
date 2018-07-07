/*
 * 磁盘接口 头文件
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef DISK_INTERFACE
#define DISK_INTERFACE

#include "std_type_define.h"
#include "disk_info.h"

/* ata通道结构 */
struct ide_channel {
	char name[8];		 	// 本ata通道名称, 如ata0,也被叫做ide0. 可以参考bochs配置文件中关于硬盘的配置。
	uint16_t port_base;		// 本通道的起始端口号
	uint8_t irq_no;		 	// 本通道所用的中断号
   
	// 暂时不使用锁和同步
	struct lock lock;		// 互斥锁
	struct semaphore disk_done;	// 硬盘处理完成.线程用这个信号量来阻塞自己，由硬盘完成后产生的中断将线程唤醒

	bool expecting_intr;		// 向硬盘发完命令后等待来自硬盘的中断
	
	struct disk devices[2];	 	// 一个通道上连接两个硬盘，一主一从
};

/* 硬盘结构 */
struct disk {
	char name[8];			   		   // 本硬盘的名称，如sda等
	struct ide_channel* my_channel;	   // 此块硬盘归属于哪个ide通道
	uint8_t dev_no;			   		   // 本硬盘是主0还是从1
   
	//暂不使用
	struct partition prim_parts[4];	   // 主分区顶多是4个
	struct partition logic_parts[8];	   // 逻辑分区数量无限,但总得有个支持的上限,那就支持8个

};

// 从磁盘读取数据到指定内存
void disk_read_block(void * mem_base_addr,uint32_t block_id, uint32_t block_num);

// 将内容写入到连续的block, 使用者自己计算如何使用
void disk_write_block(void * mem_base_addr,uint32_t mem_length, uint32_t block_id);

#endif
