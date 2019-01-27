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
bool elf_get_section(void * file_mem_addr, struct ptos_elf_section_info * s_info/* 0 .text, 1 .data, 2 .bss */){
    struct elf_header * eh_ptr = (struct elf_header *)file_mem_addr;
    uint32_t section_addr =  (uint32_t)eh_ptr->elf_shoff;
    uint32_t section_num = (uint32_t)eh_ptr->elf_shnum;
    uint32_t section_h_size = (uint32_t)eh_ptr->elf_shentsize;

    

    /*
    uint32_t i=0;
    uint32_t count = 0;
    for(;i<section_num && count<3; i++){
        
        section_addr += section_h_size;
    }
    */
}

