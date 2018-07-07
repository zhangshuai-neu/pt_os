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

//--------------------------------配置pde和pte--------------------------------------
/*
 * 设置页目录-表项
 * pde_id：指明页目录条目
 * pde_val：条目值(页表物理地址)
 */
void set_pde(uint32_t pde_id, uint32_t pde_val){
	uint32_t * pde_addr = (uint32_t *)(pde_id*PDE_SIZE + PAGE_DIR_BASE_ADDR);

	//页目录表项设置为所有权限，在页表处进行限制
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
void set_kd_pde(uint32_t pt_id, uint32_t pte_id,uint32_t pte_val, char* type){
	uint32_t * pte_addr =  PAGE_TAB_BASE_ADDR + pt_id*SIZE_4K + pte_id*PTE_SIZE;

	if(ptsc_strcmp(type,"kd") == 0){
		SET_PRESENT_BIT(pte_val);
		SET_RW_BIT(pte_val);
	} else if(ptsc_strcmp(type,"kc") == 0){
		SET_PRESENT_BIT(pte_val);
	} else if(ptsc_strcmp(type,"ud"){
		SET_PRESENT_BIT(pte_val);
		SET_RW_BIT(pte_val);
		SET_US_BIT(pte_val);
	} else if(ptsc_strcmp(type,"uc"){
		SET_PRESENT_BIT(pte_val);
		SET_US_BIT(pte_val);
	} else {
		//错误处理
		return ;
	}
	
	*pte_addr = pte_val;
}


//--------------------------------位示图-----------------------------------------
//全局变量
struct bitmap * page_bit_map_p = SIZE_1Ms + SIZE_4K + SIZE_4K*32;

//函数
void bitmap_init(){
	
}



