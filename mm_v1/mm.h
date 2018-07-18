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

#include "std_type_define.h"
#include "bitmap.h"

// 内存大小 boot/boot_parameter.inc 下有说明
#define MEM_SIZE_RECORD_ADDR	0xa000
#define PAGE_OFFSET		(12)	//偏移量

//内存位置
#define FS_ADDR	SIZE_4M

//配置pde和pte
#define PAGE_DIR_BASE_ADDR SIZE_1M							//页目录地址
#define PDE_SIZE ((uint32_t)sizeof(uint32_t))				//页目录条目大小
#define PAGE_TAB_BASE_ADDR (PAGE_DIR_BASE_ADDR+SIZE_4K)		//页表地址
#define PTE_SIZE ((uint32_t)sizeof(uint32_t))				//页表条目大小

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

//位式图
#define MEM_BITMAP_ADDR	(SIZE_1M + 132*SIZE_1K)	// 1M+132K～1M+136K
#define MEM_BITMAP_SIZE	(SIZE_4K)		// 4KB，用bit表示所有128M内存的所有页面


//--------------------------------函数声明--------------------------------------
//设置页目录-表项
void set_pde(uint32_t pde_id, uint32_t pde_val);
//设置页表-表项
void set_kd_pde(uint32_t pt_id, uint32_t pte_id,uint32_t pte_val, char* type);
//设置内核使用的内存区域（pde,pte的配置）
void set_kernel_mmap();
