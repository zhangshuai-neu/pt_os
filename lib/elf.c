/*
 * ELF文件解析
 *
 * 获取ELF文件各段的地址
 * 为加载到内存做准备
 * 
 * NOT OK!
 * 
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "std_type_define.h"
#include "elf.h"

/*
 * 
 * 假设elf文件已经加载到内存
 * 但是尚未按照期望布局进行安排
 * 需要对elf文件进行解析，找到.text .data .bss 等段的位置
 * 然后创建进程，部署进程的内存空间
 * 
 * */
 

//读取elf header

//读取 program header

uint32_t get_bss_addr(elf_header * eh){
	
}

uint32_t get_data_addr(elf_header * eh){
	
}


