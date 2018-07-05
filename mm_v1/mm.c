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

//宏定义
#define SIZE_1M ((uint32_t)1024*1024)
#define SIZE_4K ((uint32_t)1024*4)
#define SIZE_1K ((uint32_t)1024)

//全局变量
struct bitmap * page_bit_map_p = SIZE_1Ms + SIZE_4K + SIZE_4K*32;

void bitmap_init(){
	
}



