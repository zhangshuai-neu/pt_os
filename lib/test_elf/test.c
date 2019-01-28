// 测试elf程序
// elf格式的程序中读取.text|.data|.bss段在文件中的位置，虚拟地址的位置
// 为vma虚拟地址管理做准备

#include <stdio.h>
#include <stdlib.h>

#include "../elf.h"

// 打印 ptos elf section header的信息
void show_ptos_es_info(struct ptos_elf_section_info * s_info){
    printf("\nvirt_addr: %x\n",s_info->virt_addr);
	printf("section_h_addr: %x\n",s_info->section_addr);
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
	fread(file_mem_addr, file_size, 1, example_fp);

	// 3)读取
	ptos_es_info es_info_array[3];	//0 .text, 1 .data, 2 .bss
	elf_get_section(file_mem_addr, es_info_array);

	// 4)打印section信息，和 size example.elf 的结果进行比较
	for(i=0;i<3;i++)
		show_ptos_es_info(&es_info_array[i]);

	// 5)关闭文件，释放内存
	free(file_mem_addr);
	fclose(example_fp);
	
	return 0;
}
