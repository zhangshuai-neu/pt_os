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
#define PT_MAX_NUM	32						//页表最大数量

#define PHY_KERNEL_BASE_ADDR	((uint32_t)0)	//内核物理基地址
#define PHY_KERNEL_PAGE_START	(0)				//内核起始页号

#define PHY_USER_BASE_ADDR		(SIZE_1M * 4)	//用户空间物理基地址
#define PHY_USER_PAGE_START		(1024)			//用户起始页号

#define PAGE_BASE_ADDR	(SIZE_1M + SIZE_1M/2)			//page结构基地址,1MB+512KB
#define PAGE_MAX_NUM	(SIZE_4K * 8)					//32Kpage结构

//flag状态
#define	INIT_FLAGS			(uint8_t)0x00	//无标记，初始化
#define FLAGS_PAGE_LOCK		(uint8_t)1		//内核部分直接初始化为次值，不可分配
#define FLAGS_PAGE_ACTIVE	(uint8_t)2		//正在使用

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
	uint16_t virt_page;			//2-3	物理页对应的虚拟页号，虚拟地址范围0-256MB
	struct list_node page_node;	//4-12	连接到buddy中的某个list
								//		所在buddy是直接获取的（设计隐含的）
};

//1) 物理页面管理
#define BUDDY_BASE_ADDR	(SIZE_1M + SIZE_4K + SIZE_4K*32) //buddy结构的基址	
#define MAX_ORDER		11		//伙伴最大级别 2^10 * 4K = 4MB
#define BUDDY_MAX_NUM	32		//buddy_node最大数量
#define BUDDY_PAGE_NUM	1024	//一个buddy中最大页面数

//10 byte
struct free_area{
	struct list_node *pb_list_head;	//page block list head，将每个page block的头page链接起来
	uint16_t pb_num;				//page block num
};

//110 byte
typedef struct buddy_node{
	struct free_area[MAX_ORDER];	//0,1,2 ... 10 order范围
};

//根据 buddy 结构指针获取 buddy_id
#define get_buddy_id(buddy) (((uint32_t)buddy - BUDDY_BASE_ADDR)/sizeof(struct buddy_node))

//根据 buddy_id，获取对应 page block 的起始 page 结构指针
#define buddy_to_page(buddy_id)	((struct page*)PAGE_BASE_ADDR + buddy_id)

//根据 page 结构指针获取 page_id
#define get_page_id(p)	(((uint32_t)p - PAGE_BASE_ADDR)/sizeof(struct page))

//根据 page 结构id，获取对应 buddy 结构指针
#define page_to_buddy	(page_id)	((struct buddy_node*)BUDDY_BASE_ADDR + (page_id/1024))


//2) 细粒度管理


#endif
