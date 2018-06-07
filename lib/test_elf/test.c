#include <stdio.h>
#include <stdlib.h>

#include "../elf.h"

//显示elf_header的部分内容
void show_elf_header(elf_header *eh_p){
	int i=0;
	
	printf("ELF header:");
	
	printf("\n  Magic: ");
	for(i=0;i<IDENT_NUM;i++){
		printf("%02x ",(uint8_t)eh_p->elf_ident[i]);
	}
	
	printf("\n  Entry point address:  ");
	printf("0x%x ",eh_p->elf_entry);
	
	printf("\n  Start of program headers:  ");
	printf("%d bytes ",eh_p->elf_phoff);
	
	printf("\n  Start of section headers:  ");
	printf("%d bytes ",eh_p->elf_shoff);
	
	printf("\n  Number of program headers:  ");
	printf("%d  ",eh_p->elf_phnum);
	
	printf("\n");
}

//测试example.elf文件的内容
//格式为 elf_32
int main(){
	FILE * example_fp = fopen("./example.elf","rb");
	
	if(example_fp == NULL){
		printf("./example.elf is not existed!\n");
		return 0;
	}
	
	//读取 elf 文件头
	elf_header * eh_p = (elf_header *)malloc(sizeof(elf_header));
	fread(eh_p,sizeof(elf_header),1,example_fp);
	show_elf_header(eh_p);
	
	//读取 segment header
	fseek(example_fp,,SEEK_SET)
	int seg_h_num = eh_p->elf_phnum;
	
	elf_seg_header * seg_h_array = \
		(elf_seg_header *)malloc(seg_h_num*sizeof(elf_seg_header));
	
	
	//
	
	
	fclose(example_fp);
	
	return 0;
}
