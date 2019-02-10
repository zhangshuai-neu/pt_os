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
 
// 和 sc/system_call.c 中的 ptsc_strcmp 函数一致
int8_t elf_strcmp(char * str_addr_a,char * str_addr_b){
	int8_t result=0;
	if(str_addr_a!=NULL &&  str_addr_b!=NULL){
		while(*str_addr_a!='\0' && *str_addr_b!= '\0'){
			if(*str_addr_a == *str_addr_b){
				str_addr_a++;
				str_addr_b++;
			} else if(*str_addr_a > *str_addr_b){
				result=1;
				return result;
			} else {
				result=-1;
				return result;
			}
		}
		if(*str_addr_a=='\0' && *str_addr_b=='\0'){
			result=0;
		}else if(*str_addr_b=='\0'){
			result=1;
		} else{
			result=-1;
		}

		return result;
	}
	return result;
	//error处理
}

// 读取.text,.data,.bss虚拟地址和section大小
// .text.__x86.get_pc_thunk.ax
// s_info[0] .text , s_info[1] .data, s_info[2] .bss 
bool elf_get_section(void * file_mem_addr, struct ptos_elf_section_info s_info[3]){
    elf_h_ptr eh_ptr = (elf_h_ptr)file_mem_addr;

    // section header 的列表地址
    uint32_t section_h_list_addr =  (uint32_t)eh_ptr->elf_shoff + (uint32_t)file_mem_addr;
    // section 的数目
    uint32_t section_num = (uint32_t)eh_ptr->elf_shnum;
    // section header 的地址
    uint32_t section_h_size = (uint32_t)eh_ptr->elf_shentsize;

    // shsymtab section 指针
    elf_sh_ptr shsymtab_section_ptr =  \
    (elf_sh_ptr)( section_h_list_addr + (eh_ptr->elf_shstrndx)*section_h_size );

    // .shsymtab section 的内容地址
    uint32_t shsymtab_section_addr = shsymtab_section_ptr->sh_offset + (uint32_t)file_mem_addr;

    uint32_t i=0;
    uint32_t count = 0;
    uint32_t temp_section_h_addr = section_h_list_addr;
    
    for(i=0;i<section_num && count<SECTION_COUNT; i++){
        // 从.shsymtab中找到section的名字, 并将信息存放在ptos_elf_section_info中
        char * section_name = (char*)(shsymtab_section_addr + ((elf_sh_ptr)temp_section_h_addr)->sh_name );
        if( elf_strcmp(section_name,".text")==(int8_t)0 ){
            s_info[ TEXT_SECTION ].section_addr = ((elf_sh_ptr)temp_section_h_addr)->sh_offset + (uint32_t)file_mem_addr;
            s_info[ TEXT_SECTION ].virt_addr = ((elf_sh_ptr)temp_section_h_addr)->sh_addr;
            s_info[ TEXT_SECTION ].size = ((elf_sh_ptr)temp_section_h_addr)->sh_size;
            count++;
        }
        if( elf_strcmp(section_name,".text.__x86.get_pc_thunk.ax")==(int8_t)0 ){
            s_info[ TEXT_X86_SECTION ].section_addr = ((elf_sh_ptr)temp_section_h_addr)->sh_offset + (uint32_t)file_mem_addr;
            s_info[ TEXT_X86_SECTION ].virt_addr = ((elf_sh_ptr)temp_section_h_addr)->sh_addr;
            s_info[ TEXT_X86_SECTION ].size = ((elf_sh_ptr)temp_section_h_addr)->sh_size;
            count++;
        }
        if( elf_strcmp(section_name,".data")==(int8_t)0 ){
            s_info[ DATA_SECTION ].section_addr = ((elf_sh_ptr)temp_section_h_addr)->sh_offset + (uint32_t)file_mem_addr;
            s_info[ DATA_SECTION ].virt_addr = ((elf_sh_ptr)temp_section_h_addr)->sh_addr;
            s_info[ DATA_SECTION ].size = ((elf_sh_ptr)temp_section_h_addr)->sh_size;
            count++;
        }
        if( elf_strcmp(section_name,".bss")==(int8_t)0 ){
            s_info[ BSS_SECTION ].section_addr = ((elf_sh_ptr)temp_section_h_addr)->sh_offset + (uint32_t)file_mem_addr;
            s_info[ BSS_SECTION ].virt_addr = ((elf_sh_ptr)temp_section_h_addr)->sh_addr;
            s_info[ BSS_SECTION ].size = ((elf_sh_ptr)temp_section_h_addr)->sh_size;
            count++;
        }
        temp_section_h_addr += section_h_size;
        
    }
}

