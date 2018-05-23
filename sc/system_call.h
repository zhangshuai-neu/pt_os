/*
 * 系统调用(内部使用)
 * 
 * 实现系统调用需要的头文件，定义了需要的宏,类型,函数声明
 *
 * Author:Shuai Zhang
 * Email: zhangshuaiisme@gmail.com
 */

#ifndef __SYSTEM_CALL_H
#define __SYSTEM_CALL_H

//宏
#define VIEW_MEM_BASE_ADDR	0xc00b8000 	//显示内存基地址
#define VIEW_ROW_SIZE		(80*2)		//80个字符一行,一个单位需要2 byte
										//ascii 1 byte,color 1 byte
										
#define VIEW_COLUMN_MAX		79			//80列
#define VIEW_ROW_MAX		24			//25行

#include "stdint.h"

//system_call id:1
void ptsc_init_view();

//system_call id:2
void ptsc_print_str(char *str);

//system_call id:3
void ptsc_print_num16(uint32_t num);

//system_call id:4
void ptsc_memcpy(void* src_addr,void* dest_addr,uint32_t size);

//system_call id:5
void ptsc_memset(void* dest_addr,uint8_t val,uint32_t size);

//system_call id:6

//system_call id:7

#endif /*__SYSTEM_CALL_H*/





