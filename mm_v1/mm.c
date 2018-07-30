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

/*
	mm_v1方式：位示图的方式管理page
	页目录:	1MB ~ 1MB+4K
	页表:	1MB+4K ~ 1MB+4K+4K*32 = 1MB+132KB
	物理页page_bitmap: 1MB+132KB

*/
#include "mm.h"
#include "system_call.h"
#include "bitmap.h"



//全局变量 用来声明内存管理的bitmap
struct bitmap phys_mem_bitmap;
struct bitmap kernel_mem_bitmap;

/*
 * 设置页目录-表项
 * pde_id：指明页目录条目
 * pde_val：条目值(页表物理地址)
 */
void set_pde(uint32_t pde_id, uint32_t pde_val){
	uint32_t * pde_addr = (uint32_t *)(pde_id*PDE_SIZE + PAGE_DIR_BASE_ADDR);

	//页目录表项设置为所有权限，在页表处进行限制
	//结尾为7
	SET_PRESENT_BIT(pde_val);
	SET_RW_BIT(pde_val);
	SET_US_BIT(pde_val);

	*pde_addr = pde_val;
}

/*
 * 设置页表-表项
 * pt_id：指明页表
 * pte_id：指明页表条目
 * pte_val：条目值(页物理地址)
 * type：指明页表类型 "kc":kernel code, "kd":kernel data, "uc":user code, "ud":user data
 */
void set_pte(uint32_t pt_id, uint32_t pte_id,uint32_t pte_val, char* type){
	uint32_t * pte_addr =  (uint32_t *)(PAGE_TAB_BASE_ADDR + pt_id*SIZE_4K + pte_id*PTE_SIZE);
	if( ptsc_strcmp(type,"kd") == 0){
		//结尾为3
		SET_PRESENT_BIT(pte_val);
		SET_RW_BIT(pte_val);
	} else if(ptsc_strcmp(type,"kc") == 0){
		SET_PRESENT_BIT(pte_val);
	} else if(ptsc_strcmp(type,"ud") == 0){
		SET_PRESENT_BIT(pte_val);
		SET_RW_BIT(pte_val);
		SET_US_BIT(pte_val);
	} else if(ptsc_strcmp(type,"uc") == 0){
		SET_PRESENT_BIT(pte_val);
		SET_US_BIT(pte_val);
	} else {
		//错误处理
		return ;
	}

	*pte_addr = pte_val;
}


/*
 * 设置内核虚拟映射(连续的物理页面)
 * 内核的虚拟和物理地址 0～32M
 * 虚拟地址用来进行查找，最后一个地址为物理地址
 */
void set_kernel_page_mmap(uint32_t kernel_start_page_addr, \
	uint32_t phys_start_page_addr, uint32_t page_num, char* type){

	uint32_t temp_page_addr=0;
	uint32_t temp_phys_page_addr=0;
	//配置每一个页面的映射
	for(uint32_t i=0; i<page_num; i++){
		//虚拟地址，前20位用来查找pde和pte
		temp_page_addr = kernel_start_page_addr + SIZE_4K * i;
		//物理地址，最后访问的真实页面
		temp_phys_page_addr = phys_start_page_addr + SIZE_4K * i;

		//设置页目录
		set_pde(KERNEL_ADDR_TO_PDE_ID(temp_page_addr),KERNEL_ADDR_TO_PT_ADDR(temp_page_addr));
		//设置页表
		if (ptsc_strcmp(type,"kd") == 0) {
			//内核数据
			set_pte(KERNEL_ADDR_TO_PT_ID(temp_page_addr),KERNEL_ADDR_TO_PTE_ID(temp_page_addr),temp_phys_page_addr,"kd");
		} else 	if (ptsc_strcmp(type,"kc") == 0) {
			//内核代码
			set_pte(KERNEL_ADDR_TO_PT_ID(temp_page_addr),KERNEL_ADDR_TO_PTE_ID(temp_page_addr),temp_phys_page_addr,"kc");
		} else{
			//error

			return ;
		}
	}
}

/*
 * 设置用户虚拟映射(连续的物理页面)
 * 用户虚拟地址范围为 32M-128M
 */
bool set_user_page_mmap(uint32_t start_page_id, uint32_t page_num, char* type){

}

//物理内存bitmap
void phys_mem_bitmap_init(){
	phys_mem_bitmap.bits = (uint8_t *)PHYS_MEM_BITMAP_ADDR;
	phys_mem_bitmap.btmp_bytes_len = (uint32_t)PHYS_MEM_BITMAP_SIZE;
	init_bitmap(&phys_mem_bitmap);

	//set physical bitmap
	bitmap_set_cont_bit(&phys_mem_bitmap,0,PHYS_ALLOC_BIT_BEGIN_INDEX,1);
}

//内核内存bitmap
void kernel_mem_bitmap_init(){
	kernel_mem_bitmap.bits = (uint8_t *)KERNEL_MEM_BITMAP_ADDR;
	kernel_mem_bitmap.btmp_bytes_len = (uint32_t)KERNEL_MEM_BITMAP_SIZE;
	init_bitmap(&kernel_mem_bitmap);

	//set kernel bitmap,将前8M设置为1，因为kernel_mmap_init未完成了前8M的内存映射
	//所以将虚拟部分和物理完成完成
	bitmap_set_cont_bit(&kernel_mem_bitmap,0,KERNEL_ALLOC_BIT_BEGIN_INDEX,1);
}

//配置内核使用的内存区域(0-8M)
void kernel_mmap_init(){
	//0-4M已经在 boot/loader.s中配置完成

	//4-8M配置文件系统使用的内存
	set_pde(1,0x102000);

	int i;
	uint32_t pte_val = FS_ADDR;
	for(i=0;i<1024;i++){
		set_pte(1,i,pte_val,"kd\0");
		pte_val += SIZE_4K;
	}
}

void kernel_mem_init(){
	//1)kernel虚拟内存初始化
	kernel_mem_bitmap_init();
	//2)物理内存初始化
	phys_mem_bitmap_init();
	//3）创建映射
	kernel_mmap_init();
}


//--------------------------利用bitmap管理物理函数-------------------------------
/*
 * 分配连续的物理页面
 * type："kernel", "user"
 * ,为
 */
uint32_t phys_page_alloc(uint32_t page_num, char * type){
	uint32_t page_bit_index = 0;

	if(ptsc_strcmp(type,"kernel") == 0) {
		//内核物理页面，从8M开始分配
		page_bit_index = bitmap_alloc_cont_bits(&phys_mem_bitmap, KERNEL_ALLOC_BIT_BEGIN_INDEX,page_num);
		if(page_bit_index + page_num > USER_ALLOC_BIT_BEGIN_INDEX){
			//error process 申请失败，内核部分不足
			ptsc_print_str("debug_zs mm:kernel mem is not enough\n");

			return 0;
		}
	} else 	if (ptsc_strcmp(type,"user") == 0) {
		//用户物理页面，从32M开始分配
		page_bit_index  = bitmap_alloc_cont_bits(&phys_mem_bitmap, USER_ALLOC_BIT_BEGIN_INDEX,page_num);
	} else{
		//error  process

		return 0;
	}
	return page_bit_index;
}

//回收连续的物理页面
void phys_page_recycle(uint32_t start_page_id, uint32_t page_num){
	bitmap_recycle_cont_bits(&phys_mem_bitmap, start_page_id, page_num);
}

//----------------------------kernel虚拟内存管理--------------------------------
// kernel_page_alloc
// kernel_page_recycle
// 主要分成三步 1)虚拟页面分配 2)物理页面分配 3)完成虚拟物理映射
//----------------------------------------------------------------------

//内核页面申请（使用物理地址bitmap作为virtual addr地址管理方式，两者一致）
bool kernel_page_alloc(uint32_t page_num, char * type){
	//1 分配连续的虚拟页面
	uint32_t kernel_virt_page_id =  bitmap_alloc_cont_bits(&kernel_mem_bitmap, KERNEL_ALLOC_BIT_BEGIN_INDEX, page_num);
	if(kernel_virt_page_id == 0){
		ptsc_print_str("debug_zs mm: kernel bitmap alloc failed\n");
		return FALSE;
	}

	//2 分配连续的物理页面
	uint32_t phys_page_id = phys_page_alloc(page_num,"kernel");
	if(phys_page_id == 0){
		ptsc_print_str("debug_zs mm: phys bitmap alloc failed\n");
		return FALSE;
	}

	//3 建立映射
	if (ptsc_strcmp(type,"kd") == 0) {
		//内核数据
		set_kernel_page_mmap(ernel_virt_page_id * SIZE_4K, phys_page_id * SIZE_4K, page_num, "kd");
	} else 	if (ptsc_strcmp(type,"kc") == 0) {
		//内核代码
		set_kernel_page_mmap(ernel_virt_page_id * SIZE_4K, phys_page_id * SIZE_4K, page_num, "kc");
	} else{
		//error  process

		return FALSE;
	}
	return TRUE;
}

//内核页面回收
void kernel_page_recycle(uint32_t start_page_id, uint32_t page_num){
	//1 回收内核页面
	bitmap_recycle_cont_bits(&kernel_mem_bitmap, start_page_id, page_num);


	//2 回收物理页面
	phys_page_recycle( /*物理页面起始*/, page_num);


	//3 解除映射


}

/*
 * 用户虚拟页面申请
 * p_btm: 进程的bitmap，每个进程有一个,在创建进程时自动维护
 *
 */
void user_page_alloc(struct bitmap p_btm,uint32_t page_num, char * type){


	uint32_t phys_page_id = phys_page_alloc(page_num);
	if(phys_page_id == 0){
		return FALSE;
	}



}

//内核虚拟页面回收
void user_page_recycle(){


}
