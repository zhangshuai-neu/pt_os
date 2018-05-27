/*
 * 内存管理
 * 
 * 管理方式 —— 页式内存管理
 * 	0)页式管理 结构
 * 	1)物理页面管理 伙伴系统
 * 	2)细粒度管理
 * 	3)虚拟地址管理
 * 	4)维护物理页面和虚拟页面映射
 * 
 * 管理地址范围：
 * 	物理地址 4MB-128MB
 *
 * Author:Shuai Zhang
 * Email: zhangshuaiisme@gmail.com
 */

#ifndef MM_H
#define MM_H

#include "std_type_define.h"
#include "list.h"
#include "system_call.h"

#define SIZE_4K	((uint32_t)4*1024)			//4K大小
#define SIZE_1M	((uint32_t)1024*1024)		//1M大小

// 0)页式管理
#define PGD_ADDR	SIZE_1M					//页目录地址
#define PT_ADDR		(SIZE_1M + SIZE_4K)		//页表地址
#define PT_MAX_NUM	32	//页表最大数量



/*
 * 每个物理页都有一个page结构,这个结构要尽可能的小
 * 128MB内存需要32K个page结构
 * 
 * 我的设计
 * 	使用限制：每一个page只能由一个进程使用
 * 	大小限制：结构大小不能超过16 byte
 * 
 * */
struct page{
	uint8_t flags;				//1		page状态，状态号0x01 0x02 0x04 ...
	uint16_t virt_page;			//2-3	物理页对应的虚拟页地址，虚拟地址范围0-256MB
	struct list_node page_node;	//4-12	连接到buddy中的某个free_list
								//		所在buddy是直接获取的（设计隐含的）
};

#define PAGE_BASE_ADDR	(SIZE_1M + SIZE_1M/2)			//page结构基地址,1MB+512KB
#define PAGE_MAX_NUM	(SIZE_4K * 8)					//32Kpage结构
#define PAGE_SIZE		((uint32_t)sizeof(struct page))	//page结构大小

//flag状态
#define	INIT_FLAGS	0x00		//
#define FLAGS_PAGE_LOCK		1	//内核部分直接初始化为次值，不可分配，已被使用
#define FLAGS_PAGE_ACTIVE	2	//

#define INIT_KERNEL_VIRT_PAGE	0x0000		//


//1) 物理页面管理
#define US_START_ADDR	(4*SIZE_1M)		//user space起始地址 4MB
#define US_END_ADDR 	(128*SIZE_1M)	//user space结束地址 128M

#define BUDDY_BASE_ADDR	(SIZE_1M + SIZE_4K + SIZE_4K*32) //buddy结构的基址	
#define MAX_ORDER		11		//伙伴最大级别 2^10 * 4K = 4MB
#define BUDDY_MAX_NUM	32		//buddy_node最大数量
#define BUDDY_PAGE_NUM	1024	//一个buddy中最大页面数

//2 byte
struct free_area{
	uint16_t free_node_num;		//可分配页块数
};

//22 byte
typedef struct buddy_node{
	struct free_area[MAX_ORDER];	//0,1,2 ... 10 order范围
};



//2) 细粒度管理


#endif
