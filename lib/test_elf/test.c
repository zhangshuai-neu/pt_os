// 测试elf程序
// elf格式的程序中读取.text|.data|.bss段在文件中的位置，虚拟地址的位置
// 为vma虚拟地址管理做准备

#include <stdio.h>
#include <stdlib.h>

#include "../elf.h"

int8_t ptsc_strcmp(char * str_addr_a,char * str_addr_b){
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

// 打印 ptos elf section header的信息
void show_ptos_es_info(struct ptos_elf_section_info * s_info){
    printf("\nvirt_addr: %x\n",s_info->virt_addr);
	printf("section_addr: %x\n",s_info->section_addr);
	printf("size: %d\n",s_info->size);
}

//测试example.elf文件的内容
//格式为 elf_32
int main(){
	int i=0;
	FILE * example_fp = fopen("./example.elf","rb");
	
	if(example_fp == NULL){
		printf("./example.elf is not existed!\n");
		return 0;
	}

	// 读取程序到内存
	// 1)获取文件大小
	fseek(example_fp, 0, SEEK_END);
	long file_size = ftell(example_fp);	
	printf("file_size : %d\n",file_size);

	// 2)读取到内存
	void * file_mem_addr = (void*)malloc(file_size);
	fseek(example_fp, 0, SEEK_SET);
	fread(file_mem_addr, 1, file_size, example_fp);

	struct elf_header * eh_ptr = (struct elf_header *)file_mem_addr;
    uint32_t section_addr =  eh_ptr->elf_shoff + (uint32_t)eh_ptr;
    uint32_t section_num = eh_ptr->elf_shnum;
    uint32_t section_h_size = eh_ptr->elf_shentsize;

	printf("section_addr: %d\n",section_addr);
	printf("section_num: %d\n",section_num);
	printf("section_h_size: %d\n",section_h_size);

	// 找到.shstrtab section
	section_addr = section_addr + (eh_ptr->elf_shstrndx)*section_h_size;
	struct elf_section_header * sh_ptr = (struct elf_section_header *)section_addr;
	printf("sh_name: %d\n", sh_ptr->sh_name);
	



/*
	// 3)读取
	ptos_es_info es_info_array[3];	//0 .text, 1 .data, 2 .bss
	elf_get_section(file_mem_addr, es_info_array);

	// 4)打印section信息，和 size example.elf 的结果进行比较
	for(i=0;i<3;i++)
		show_ptos_es_info(&es_info_array[i]);
*/
	// 5)关闭文件，释放内存
	free(file_mem_addr);
	fclose(example_fp);
	
	return 0;
}
