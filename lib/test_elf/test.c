// 测试elf程序
// elf格式的程序中读取.text|.data|.bss段在文件中的位置，虚拟地址的位置
// 为vma虚拟地址管理做准备

#include <stdio.h>
#include <stdlib.h>

#include "../elf.h"

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
	void * file_mem_addr = 0;

	// 关闭文件
	fclose(example_fp);
	
	return 0;
}
