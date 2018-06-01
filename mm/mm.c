/*
 * 内存管理
 * 
 * 使用页式内存管理
 * 	1)物理页面管理 伙伴系统
 * 	2)细粒度管理
 * 	3)虚拟地址管理
 * 	4)维护物理页面和虚拟页面映射
 *
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
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
		buddy_array[i].free_area[MAX_ORDER-1].pb_list_head = buddy_to_page(i)->page_node)
		buddy_array[i].free_area[MAX_ORDER-1].pb_num = 1;
	}
}



static inline uint16_t get_pb_key(struct list_node * node){
	return get_page_id(list_entry(node,struct page,page_node));
}

//将pb_list，变成有序的(从小到大)
//使用插入排序，因为始终在维持，所以只要修改少次，效率比较高
void sort_pb_list(struct list_node* list_head){
	//未排序list
	struct list_node * unsort_list_head = list_head->next;
	struct list_node * move_node = NULL;
	
	//排序list
	struct list_node * sort_list_head  = list_head;
	struct list_node * sort_list_tail  = list_head;
	
	//move_node 要插入到 insert_location之前	
	struct list_node * insert_location = NULL;
	
	list_remove(list_head);
	list_init(sort_list_head);
	
	//判断条件
	while(move_node != ){
		move_node = unsort_list_head;
		unsort_list_head = unsort_list_head->next;
		
		
		insert_location = sort_list_head;
		while(insert_location =! sort_list_head){
			if(get_pb_key(insert_location)  > get_pb_key(move_node)) break;
			insert_location = insert_location->next;
		}
		//插入到和啊的
		list_remove(move_node);
		list_insert(insert_location,move_node);
		
		
		
	}
	

}

/*
 * 将一个大的pb_list拆解成两个小的                                                                                        
 * sp_order 要拆分order，拆解该order的头部page block
 * 
 * notice: 可行性判断，由调用者解决
 * 
 * */
bool pb_list_separation(struct buddy_node *bp, uint8_t sp_order){
	//高order的list
	struct list_node *high_list_head = bp->free_area[sp_order].pb_list_head;
	//第order的list
	struct list_node *low_list_head = bp->free_area[sp_order-1].pb_list_head;
	
	//新的两的page_block的间隔
	uint32_t page_interval = (uint32_t(1))<<(sp_order-1);
	
	//找到两个新的page block的头page指针
	struct page *node1 = high_list;
	struct page *node2 = *(page_array[ get_page_id(node1) + page_interval ]);
	
	//高order 删除一个pb
	list_remove(high_list_head);
	bp->free_area[sp_order].pb_num -= 1;
	
	//低order 添加两个pb, 保持顺序
	list_insert(low_list_head,head2);
	list_insert(low_list_head,head1);
	bp->free_area[sp_order-1].pb_num += 2;
}

/*
 * 从buddy中申请order级别的page block
 * buddy_id 
 * order
 * ret_p 为返回page结构指针
 * 
 * */
bool buddy_pb_alloc(uint8_t buddy_id, uint8_t app_order, struct page *ret_p){
	struct buddy_node *bp = buddy_array + buddy_id;
	bool result=FALSE;
	
	if(app_order < MAX_ORDER && bp->free_area[app_order].pb_num > 0){
		result = TRUE;
	}
	else {
		uint8_t i_order = MAX_ORDER-1;
		for(; i_order > app_order; i_order--){
			//该级没有空闲pb，则一直跳出
			if(bp->free_area[i_order].pb_num == 0) continue;
			
			result = pb_list_separation(bp,i_order);
		}
	}
	//page_id赋值
	if(result){
		ret_p = bp->free_area[app_order].pb_list_head;	//pb_list的头部
		list_remove(ret_p);								//从pb_list中删除
	}
	else{
		ret_p = NULL;
	}
	
	return result;
}

//将低order的连续pb合并成一个高order的pb
void pb_list_combination(struct buddy_node *bp, uint8_t cb_order){
	struct page* list_head = bp->free_area[cb_order].pb_list_head;
	
	
	
}

//自动回收所有能回收的 page block
bool buddy_pb_recycle(uint8_t buddy_id){
	
	
}



// 2)细粒度管理

