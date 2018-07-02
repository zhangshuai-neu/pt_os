/*
 * 磁盘接口
 * 
 * 用来访问硬盘
 * 
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef DISK_INTERFACE
#define DISK_INTERFACE

#include "std_type_define.h"

/*
 * 从磁盘读取数据到指定内存
 * 
 * */
void disk_read(void * mem_base_addr,uint32_t block_id, uint32_t block_num);


#endif
