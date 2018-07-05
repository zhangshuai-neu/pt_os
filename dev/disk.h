#ifndef DISK_H
#define DISK_H

/* ata通道结构 */
struct ide_channel {
   char name[8];		 	 // 本ata通道名称, 如ata0,也被叫做ide0. 可以参考bochs配置文件中关于硬盘的配置。
   uint16_t port_base;		 // 本通道的起始端口号
   uint8_t irq_no;		     // 本通道所用的中断号
   struct lock lock;
   bool expecting_intr;		 	 // 向硬盘发完命令后等待来自硬盘的中断
   struct semaphore disk_done;	 // 硬盘处理完成.线程用这个信号量来阻塞自己，由硬盘完成后产生的中断将线程唤醒
   struct disk devices[2];	 	 // 一个通道上连接两个硬盘，一主一从
};


#endif
