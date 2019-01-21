/*
 * ELF文件解析
 *
 * 获取ELF文件各段的地址
 * 为加载到内存做准备
 * 
 * 读取elf文件的.text .data .bss三个section
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
 
 // 读取.text,.data,.bss虚拟地址和section大小
 bool elf_get_section(void * file_mem_addr, struct ptos_elf_section_info * s_info){
     
 }

