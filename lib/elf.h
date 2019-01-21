/*
 * ELF文件解析
 *
 * 获取ELF文件各段的地址
 * 为加载到内存做准备
 * 
 * 我只添加了elf文件头和program段头，其他的部分与加载无关
 * 其他内容可以在 /usr/include/elf.h 中查看（如果你感兴趣）
 * 
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef ELF_H
#define ELF_H

#include "std_type_define.h"

//============================================
// elf文件格式需要的定义
//============================================
typedef uint16_t elf_half;		//半字
typedef uint32_t elf_word;		//字
typedef uint32_t elf_addr;		//地址
typedef uint32_t elf_off;		  //偏移

#define IDENT_NUM ((uint32_t)16)

/* elf 文件头 */
struct elf_header
{
  uint8_t	elf_ident[IDENT_NUM];	/* 魔数和一些信息 */
  elf_half	elf_type;			  /* ELF文件类型 */
  elf_half	elf_machine;		/* 体系结构 */
  elf_word	elf_version;		/* ELF版本号 */
  elf_addr	elf_entry;			/* 虚拟地址 */
  elf_off	  elf_phoff;			/* 程序头表在文件中的偏移 */
  elf_off	  elf_shoff;			/* 段表在文件中的偏移 */
  elf_word	elf_flags;			/* ELF标志位 */
  elf_half	elf_ehsize;			/* ELF头文件本身大小 */
  elf_half	elf_phentsize;	/* 程序头表条目大小 */
  elf_half	elf_phnum;			/* 程序头表计数 */
  elf_half	elf_shentsize;	/* 段表条目大小 */
  elf_half	elf_shnum;			/* 段表条目数量 */
  elf_half	elf_shstrndx;		/* 字符串表 */
};

/* section 段 */
struct elf_section_header
{
  elf_word	sh_name;		/* Section name (string tbl index) */
  elf_word	sh_type;		/* Section type */
  elf_word	sh_flags;		/* Section flags */
  elf_addr	sh_addr;		/* 虚拟地址 Section virtual addr at execution */
  elf_off 	sh_offset;	    /* 文件偏移 Section file offset */
  elf_word	sh_size;		/* section的大小 Section size in bytes */
  elf_word	sh_link;		/* Link to another section */
  elf_word	sh_info;		/* Additional section information */
  elf_word	sh_addralign;	/* Section alignment */
  elf_word	sh_entsize;		/* Entry size if section holds table */
} ;

//===============================
// pt_os 读取section需要的结构
//===============================
typedef struct ptos_elf_section_info{
    uint32_t virt_addr;
    uint32_t size;
} ptos_es_info;

#endif
