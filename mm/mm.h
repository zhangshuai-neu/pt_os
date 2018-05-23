/*
 * 内存管理
 * 
 * 管理方式 —— 页式内存管理
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

//头文件

//宏

#define MEM_MAX_SIZE ((uint32_t)128*1024*1024)	//内存最大128MB
#define BUDDY_BASE_ADDR	((void *))				//伙伴系统起始地址


/*
 * 物理页面管理
 * buddy结构
 * 
 * */



#endif
