/*
 * 磁盘接口 实现
 *
 * Author:《操作系统真相还原》
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

/* 向硬盘控制器写入起始扇区地址及要读写的扇区数 */
static void select_sector(struct disk* hd, uint32_t lba, uint8_t sec_cnt) {
   struct ide_channel* channel = hd->my_channel;

   /* 写入要读写的扇区数*/
   outb(reg_sect_cnt(channel), sec_cnt);	 // 如果sec_cnt为0,则表示写入256个扇区

   /* 写入lba地址(即扇区号) */
   outb(reg_lba_l(channel), lba);		 // lba地址的低8位,不用单独取出低8位.outb函数中的汇编指令outb %b0, %w1会只用al。
   outb(reg_lba_m(channel), lba >> 8);		 // lba地址的8~15位
   outb(reg_lba_h(channel), lba >> 16);		 // lba地址的16~23位

   /* 因为lba地址的24~27位要存储在device寄存器的0～3位,
    * 无法单独写入这4位,所以在此处把device寄存器再重新写入一次*/
   outb(reg_dev(channel), BIT_DEV_MBS | BIT_DEV_LBA | (hd->dev_no == 1 ? BIT_DEV_DEV : 0) | lba >> 24);
}

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
/*
void disk_write_block(void * mem_base_addr, uint32_t mem_length, uint32_t block_id){
	// 1 先选择操作的硬盘
	select_disk(hd);

	uint32_t secs_op;		 // 每次操作的扇区数
	uint32_t secs_done = 0;	 // 已完成的扇区数
	while(secs_done < sec_cnt) {
		if ((secs_done + 256) <= sec_cnt) {
			secs_op = 256;
		} else {
			secs_op = sec_cnt - secs_done;
		}

        // 2 写入待写入的扇区数和起始扇区号
		select_sector(hd, lba + secs_done, secs_op);	// 先将待读的块号lba地址和待读入的扇区数写入lba寄存器

        // 3 执行的命令写入reg_cmd寄存器 
		cmd_out(hd->my_channel, CMD_WRITE_SECTOR);	    // 准备开始写数据

        // 4 检测硬盘状态是否可读 
		if (!busy_wait(hd)) {	// 若失败
			char error[64];
			sprintf(error, "%s write sector %d failed!!!!!!\n", hd->name, lba);
			PANIC(error);
		}

        // 5 将数据写入硬盘 
		write2sector(hd, (void*)((uint32_t)buf + secs_done * 512), secs_op);

		// 在硬盘响应期间阻塞自己
		sema_down(&hd->my_channel->disk_done);
		secs_done += secs_op;
	}
}
*/

/* 
 * 硬盘数据结构初始化 
 * 使用一个ide接口连接一个主硬盘
 * */
void ide_init() { 
	//初始化信道
	*channel_cnt = 1;	//通道数量，默认为1
	ptsc_strcpy(channel->name,"ide0");
	//为ide通道初始化端口基址及中断向量
	channel->port_base	 = 0x1f0;	   // ide0通道的起始端口号是0x1f0
	channel->irq_no	 = 0x20 + 14;	   // 从片8259a上倒数第二的中断引脚,也就是ide0通道的的中断向量号
	channel->expecting_intr = FALSE;   // 未向硬盘写入指令时不期待硬盘的中断
	
	//初始化硬盘(一个channel实际上可以链接多个硬盘)
	struct disk *hd = &(channel->device);
	ptsc_strcpy(hd->name,"hda0");
	hd->my_channel = channel;
	hd->dev_no = 0;
	
	return ;
}

