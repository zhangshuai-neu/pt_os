/*
 * 磁盘接口
 * 
 *
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "disk_interface.h"

/*
 * 从磁盘读取连续的block到指定内存
 *
 * mem_base_addr : 目的内存基地址（物理地址）
 * block_id : block起始号
 * block_num : block数量
 * 
 * */
void disk_read_block(void * mem_base_addr,uint32_t block_id, uint32_t block_num){


}

/*
 * 将内容写入到连续的 block
 * 
 * mem_base_addr : 源内存基地址（物理地址）
 * mem_length ： 读取的长度，根据长度
 * block_id : block起始号
 * 
 * */
void disk_write_block(void * mem_base_addr,uint32_t mem_length, uint32_t block_id){


}
