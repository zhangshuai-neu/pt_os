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
	uint16_t i=0;
	for(i=0; i<PAGE_MAX_NUM; i++){
			page_array[i].flags=0
			page_array[i].freelist_id=0;
			page_array[i].virt_page_addr=0;
			page_array[i].process_id=0;
	}
}


// 1)物理页面管理 伙伴系统
struct buddy_node * buddy_array = BUDDY_BASE_ADDR;

void buddy_init(){
	
}

// 2)细粒度管理

