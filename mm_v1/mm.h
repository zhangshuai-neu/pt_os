/*
 * 内存管理
 *
 * 管理方式 —— 使用位示图的方式进行管理
 *
 * 管理地址范围：
 * 	物理地址 4MB-128MB
 *
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */
#ifndef MM_H
#define MM_H

#include "std_type_define.h"
#include "bitmap.h"

// 内存大小 boot/boot_parameter.inc 下有说明
#define MEM_SIZE_RECORD_ADDR	0xa000
#define PAGE_OFFSET		(12)	//偏移量

//内存位置
#define FS_ADDR	SIZE_4M

//配置pde和pte
#define PAGE_DIR_BASE_ADDR SIZE_1M							         //页目录地址
#define PDE_SIZE ((uint32_t)sizeof(uint32_t))				     //页目录条目大小
#define PAGE_TAB_BASE_ADDR (PAGE_DIR_BASE_ADDR+SIZE_4K)	 //页表地址
#define PTE_SIZE ((uint32_t)sizeof(uint32_t))				     //页表条目大小

//内核地址 => 虚拟-物理配置索引
#define KERNEL_ADDR_TO_PDE_ID(ka)  (((uint32_t)0xffc00000 & (uint32_t)ka) >> 22)
#define KERNEL_ADDR_TO_PT_ID(ka)   ((uint32_t)ka / SIZE_4M)
#define KERNEL_ADDR_TO_PT_ADDR(ka)  (KERNEL_ADDR_TO_PT_ID(ka) * SIZE_4K + SIZE_1M + SIZE_4K)
#define KERNEL_ADDR_TO_PTE_ID(ka)  (((uint32_t)0x003FF000 & (uint32_t)ka) >> 12)



//页表相关属性
#define PAGE_PRESENT_BIT    ((uint32_t)1)	//存在位（为1在物理内存，为0不在）
#define PAGE_RW_BIT			((uint32_t)2)	//读写位，为0可读不可写，为1可读可写
#define PAGE_US_BIT			((uint32_t)4)	//用户|特权位，为0特权，为1用户

#define SET_PRESENT_BIT(var) 	var = (var | PAGE_PRESENT_BIT)
#define CLEAR_PRESENT_BIT(var) 	var = (var & (~PAGE_PRESENT_BIT))
#define SET_RW_BIT(var) 		var = (var | PAGE_RW_BIT)
#define CLEAR_RW_BIT(var)		var = (var & (~PAGE_RW_BIT))
#define SET_US_BIT(var) 		var = (var | PAGE_US_BIT)
#define CLEAR_US_BIT(var)		var = (var & (~PAGE_US_BIT))

//位示图
//物理、内核位示图
#define PHYS_MEM_BITMAP_ADDR	(SIZE_1M + 132*SIZE_1K)	// 1M+132K～1M+136K
#define PHYS_MEM_BITMAP_SIZE	(SIZE_4K)		// 4KB，用bit表示所有128M内存的所有页面
//用户虚拟地址位示图
#define USER_MEM_BITMAP_ADDR	(PHYS_MEM_BITMAP_ADDR + PHYS_MEM_BITMAP_SIZE)
#define USER_MEM_BITMAP_SIZE	(SIZE_4K)		// 4KB，用bit表示所有128M内存的所有页面

#define ALLOC_BIT_BEGIN_INDEX ((uint32_t)2048)  //分配的物理起始索引

//内核使用内存(虚拟地址划分，0~128M)
#define KERNEL_VIRTUAL_BASE_ADDR  ((uint32_t)0)

//用户使用内存(虚拟地址范围，128M~256M)
#define USER_VIRTUAL_BASE_ADDR  ((uint32_t)128*SIZE_1M)



//--------------------------------函数声明--------------------------------------
//设置内核使用的内存区域（pde,pte的配置）
void kernel_mmap_init();
//内存bitmap初始化
void mem_bitmap_init();
//分配内存内存,type=="kd" or "kc", 代表数据和代码
bool kernel_page_alloc(uint32_t page_num, char * type);
//回收内存
void kernel_page_recycle(uint32_t start_page_id, uint32_t page_num);








#endif
