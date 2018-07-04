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

// 从磁盘读取数据到指定内存
void disk_read(void * mem_base_addr,uint32_t block_id, uint32_t block_num);

// 将内容写入到连续的block, 使用者自己计算如何使用
void disk_write_block(void * mem_base_addr,uint32_t mem_length, uint32_t block_id);

#endif
