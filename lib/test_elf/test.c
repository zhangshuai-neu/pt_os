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

//显示segment header
void show_seg_header(elf_seg_header * seg_h_array, int seg_num){
	int i=0;
	
	printf("\nELF Segment header:\n");
	for(i=0; i<seg_num; i++){
		printf("  Segment %d\n",i);
		
		
		printf("    Type:  ");
		printf("%x\n",seg_h_array[i].seg_type);
		switch(){
			
		}
		
		printf("    Offset:  ");
		printf("%x\n",seg_h_array[i].seg_offset);
		
		printf("    Segment Filesize:  ");
		printf("%x\n",seg_h_array[i].seg_filesz);
		
		printf("    Memory Filesize:  ");
		printf("%x\n",seg_h_array[i].seg_memsz);
		
	}
	printf("\n");
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
	
	//读取 elf 文件头
	elf_header * eh_p = (elf_header *)malloc(sizeof(elf_header));
	fread(eh_p,sizeof(elf_header),1,example_fp);
	show_elf_header(eh_p);
	
	//读取 segment 头部
	int seg_h_offset = eh_p->elf_phoff;
	int seg_h_num = eh_p->elf_phnum;
		
	elf_seg_header * seg_h_array = (elf_seg_header *)malloc(seg_h_num*sizeof(elf_seg_header));
		
	fseek(example_fp,seg_h_offset,SEEK_SET);	
	fread(seg_h_array,sizeof(elf_seg_header),seg_h_num,example_fp);
	
	show_seg_header(seg_h_array,seg_h_num);

	
	//
	
	free(eh_p);
	free(seg_h_array);
	
	fclose(example_fp);
	
	return 0;
}
