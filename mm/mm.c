/*
 * 内存管理
 * 
 * 使用页式内存管理
 * 	1)物理页面管理 伙伴系统
 * 	2)细粒度管理
 * 	3)虚拟地址管理
 * 	4)维护物理页面和虚拟页面映射
 *
 * Author:Shuai Zhang
 * Email: zhangshuaiisme@gmail.com
 */
 
#include "std_type_define.h"
#include "mm.h"


// 0)页式管理
struct page * page_array = PAGE_STRUCT_BASE_ADDR;
void page_struct_init(){
	//页号
	uint16_t i=0;	
	//内核空间
	for(i=PHY_KERNEL_PAGE_START;i<PHY_USER_PAGE_START;i++){
		page_array[i].flags = INIT_FLAGS | FLAGS_PAGE_LOCK | FLAGS_PAGE_ACTIVE ;
		//内核的虚拟和物理地址一致
		page_array[i].virt_page = i;
		list_init(page_array[i].page_node);
	}
	
	//用户空间
	for(i=PHY_USER_PAGE_START; i<PAGE_MAX_NUM; i++){
		page_array[i].flags = INIT_FLAGS
		page_array[i].virt_page = 0;
	}
}


// 1)物理页面管理 伙伴系统
struct buddy_node * buddy_array = BUDDY_BASE_ADDR;

void buddy_init(){
	
}

// 2)细粒度管理

