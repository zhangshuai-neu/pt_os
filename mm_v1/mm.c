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

//--------------------------------配置pde和pte--------------------------------------
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

/*
 * 设置内核虚拟映射(连续的物理页面)
 * 内核的虚拟和物理地址完全对应 0～128M
 */
void set_kernel_page_mmap(uint32_t kernel_start_page_addr, uint32_t page_num, char* type){
	uint32_t temp_page_addr=0;
	//配置每一个页面的映射
	for(uint32_t i=0; i<page_num; i++){
		temp_page_addr = kernel_start_page_addr + SIZE_4K * i;
		//设置页目录
		set_pde(KERNEL_ADDR_TO_PDE_ID(temp_page_addr),KERNEL_ADDR_TO_PT_ADDR(temp_page_addr));

		//设置页表
		if (ptsc_strcmp(type,"kd") == 0) {
			//内核数据
			set_pte(KERNEL_ADDR_TO_PT_ID(temp_page_addr),KERNEL_ADDR_TO_PTE_ID(temp_page_addr),temp_page_addr,"kd");
		} else 	if (ptsc_strcmp(type,"kc") == 0) {
			//内核代码
			set_pte(KERNEL_ADDR_TO_PT_ID(temp_page_addr),KERNEL_ADDR_TO_PTE_ID(temp_page_addr),temp_page_addr,"kc");
		} else{
			//error

			return ;
		}

	}
}

/*
 * 设置用户虚拟映射(连续的物理页面)
 * 用户虚拟地址范围为 128M~256M
 */
bool set_user_page_mmap(uint32_t start_page_id, uint32_t page_num, char* type){

}


//--------------------------------位示图-----------------------------------------
//全局变量 用来声明内存管理的bitmap
struct bitmap phys_mem_bitmap;
struct bitmap * kernel_mem_bitmap = &phys_mem_bitmap;

//函数
void mem_bitmap_init(){
	phys_mem_bitmap.bits = (uint8_t *)PHYS_MEM_BITMAP_ADDR;
	phys_mem_bitmap.btmp_bytes_len = (uint32_t)PHYS_MEM_BITMAP_SIZE;
	init_bitmap(&mem_bitmap);

	//set kernel bitmap
	bitmap_set_cont_bit(&phys_mem_bitmap,0,2048,1);

}

//--------------------------利用bitmap管理物理函数-------------------------------
//分配连续的物理页面
uint32_t phys_page_alloc(uint32_t page_num){
	uint32_t page_bit_index = bitmap_alloc_cont_bits(&phys_mem_bitmap, ALLOC_BIT_BEGIN_INDEX,page_num);
	return page_bit_index;
}

//回收连续的物理页面
void phys_page_recycle(uint32_t start_page_id, uint32_t page_num){
	bitmap_recycle_cont_bits(&phys_mem_bitmap, start_page_id, page_num);
}

//内核虚拟页面申请（使用物理地址bitmap作为virtual addr地址管理方式，两者一致）
bool kernel_page_alloc(uint32_t page_num, char * type){
	uint32_t page_id = phys_page_alloc(page_num);
	if(page_id == 0){
		return FALSE;
	}

	uint32_t page_addr = page_id * SIZE_4K;

	if( ptsc_strcmp(type,"kd")==0 ){
		//内核数据
		set_kernel_page_mmap(page_addr,page_num,"kd");
	}
	if( ptsc_strcmp(type,"kc")==0 ){
		//内核代码
		set_kernel_page_mmap(page_addr,page_num,"kc");
	}

	return TRUE;
}

//内核虚拟页面回收
void kernel_page_recycle(uint32_t start_page_id, uint32_t page_num){
	phys_page_recycle(uint32_t start_page_id, uint32_t page_num);
}

//用户虚拟页面申请
void user_page_alloc(){


}

//内核虚拟页面回收
void user_page_recycle(){


}
