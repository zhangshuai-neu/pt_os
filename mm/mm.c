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

/* 提示
struct page{
	uint8_t flags;				//1		page状态，状态号0x01 0x02 0x04 ...
	uint16_t virt_page;			//2-3	物理页对应的虚拟页号，虚拟地址范围0-256MB
	struct list_node page_node;	//4-12	连接到buddy中的某个list
								//		所在buddy是直接获取的（设计隐含的）
};
*/

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
		list_init(page_array[i].page_node);
	}
}


/* 提示
//6 byte
struct free_area{
	struct list_node *pb_head;	//page block list head，将每个page block的头page链接起来
	uint16_t pb_num;			//page block num
};
*/

// 1)物理页面管理 伙伴系统
struct buddy_node * buddy_array = BUDDY_BASE_ADDR;
void buddy_init(){
	uint8_t i=0;
	struct list_node * head;
	for(i=0; i<BUDDY_MAX_NUM; i++){
		head = &(buddy_array[i].free_area[MAX_ORDER-1].pb_list_head);
		list_init(head);
		list_insert(head,&(buddy_to_page(i)->page_node));
		buddy_array[i].free_area[MAX_ORDER-1].pb_num = 1;
	}
}

/*
 * 将一个大的pb_list拆解成两个小的
 * sp_order 要拆分order，拆解该order的头部page block
 * 
 * */

bool pb_list_separation(uint8_t buddy_id, uint8_t sp_order){
	if(sp_order >= MAX_ORDER || sp_order == 0){
		return FALSE;
	}
	
	struct buddy_node *bp = buddy_array+buddy_id;
	struct list_node *high_list = bp.free_area[sp_order].pb_head;
	struct list_node *low_list = bp.free_area[sp_order-1].pb_head;
	uint32_t new_block_size = (uint32_t(1))<<(sp_order-1);
	
	//高order 修改
	
	
	
	//低order 修改
	

}

/*
 * 从buddy中申请order级别的page block
 * buddy_id 
 * order
 * page_id 为返回值
 * */
bool buddy_alloc(uint8_t buddy_id, uint8_t order, uint16_t *page_id){
	if(order >= MAX_ORDER){
		//order 不合法
		return FALSE;
	}
	
	struct buddy_node *bp = buddy_array+buddy_id;
	uint8_t i_order = MAX_ORDER-1;
	//
	for(){
		
		
	}
	return TRUE;
}

//自动回收所有能回收的 page block
bool buddy_recycle(){
	
}



// 2)细粒度管理

