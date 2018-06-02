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

// 1)物理页面管理 伙伴系统
struct buddy_node * buddy_array = BUDDY_BASE_ADDR;
void buddy_init(){
	uint8_t i=0;
	struct list_node * head;
	for(i=0; i<BUDDY_MAX_NUM; i++){
		ptsc_memset(&(buddy_array[i].free_area[0]),0,10);
		buddy_array[i].free_area[MAX_ORDER-1].pb_list_head = buddy_to_page(i)->page_node)
		buddy_array[i].free_area[MAX_ORDER-1].pb_num = 1;
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
 * ret_p 为返回page结构指针,page block的head
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

static inline uint32_t get_pb_key(struct list_node * node){
	return (uint32_t)get_page_id(list_entry(node,struct page,page_node));
}

//将pb_list，变成有序的(从小到大)
//使用插入排序，因为始终在维持，所以只要修改少次，效率比较高
struct list_node* sort_pb_list(struct list_node* list_head){
	//未排序list
	struct list_node * unsort_list_head = list_head->next;
	struct list_node * move_node = NULL;
	struct list_node end_node;	//只是结束标志
	
	//排序list
	struct list_node * sort_list_head  = list_head;
	struct list_node * sort_list_tail  = list_head;
	
	//move_node 要插入到 insert_location之前	
	struct list_node * insert_location = NULL;
	
	//初始化unsort_list
	list_remove(list_head);
	list_insert(unsort_list_head,&end_node);	
	//初始化sort_list
	list_init(sort_list_head);
	
	while(move_node != &end_node){
		move_node = unsort_list_head;
		unsort_list_head = unsort_list_head->next;
		
		//比head小
		if( get_pb_key(move_node) < get_pb_key(sort_list_head) ){
			list_remove(move_node);
			list_insert(sort_list_head,move_node);
			sort_list_head = sort_list_head->prev;
			break;
		}
		//比tail大
		if( get_pb_key(sort_list_tail) < get_pb_key(move_node) ){
			list_remove(move_node);
			list_insert(sort_list_head,move_node);
			sort_list_tail = sort_list_tail->next;
			break;
		}
		//在head和tail之间
		insert_location = sort_list_head->next;
		while(insert_location != sort_list_tail){
			if(get_pb_key(insert_location) > get_pb_key(move_node)) break;
			insert_location = insert_location->next;
		}
		/* 删除和插入
		 * 比tail小也在此处处理，因为到此处时move_node一定小于sort_list_tail
		 */
		list_remove(move_node);
		list_insert(insert_location,move_node);
	}
	
	return sort_list_head;
}

//至少有两个pb才能合并
//将低order的连续pb合并成一个高order的pb
void pb_list_combination(struct buddy_node *bp, uint8_t cb_order){
	//pb size
	uint32_t bs = (uint32_t)1 << cb_order;	
	//有序 pb list
	struct list_node* new_list_head = bp->free_area[cb_order].pb_list_head;
	new_list_head = sort_pb_list(new_list_head);
	//pb 数量
	new_pb_num = bp->free_area[cb_order].pb_num;
	
	
	
	struct list_node* cb1 = new_list_head;
	struct list_node* cb2 = new_list_head->next;
	
	while(new_pb_num >= 2 && cb1 != new_list_head->prev ){
		if( get_pb_key(cb2) - get_pb_key(cb1) == bs ){	
			//相邻，应该合并
			cb2 = cb2->next;
			
			//修改本级
			list_remove(cb1);
			list_remove(cb1->next);
			new_pb_num-=2;
			
			//修改高级
			bp->free_area[cb_order+1].pb_num += 1;
			list_insert(bp->free_area[cb_order+1].pb_list_head,cb1);
			
			
			if(cb1 == new_list_head)
				new_list_head = cb2;
		}
		cb1 = cb2;
		cb2 = cb2->next;
	}
	
	//修改本级free_area
	if(new_pb_num == 0)
		bp->free_area[cb_order].pb_list_head = NULL;
	else
		bp->free_area[cb_order].pb_list_head = new_list_head;
		
	bp->free_area[cb_order].pb_num = new_pb_num;
	
	
}

//自动指定buddy回收所有能回收的 page block
bool buddy_pb_recycle(uint8_t buddy_id){
	uint8_t i=0;
	for(i=0; i < MAX_ORDER-1w ;i++){
		if(buddy_array[buddy_id].pb_num > 2)
			pb_list_combination(&(buddy_array[buddy_id]),i);
	}
}


// 2)细粒度管理

