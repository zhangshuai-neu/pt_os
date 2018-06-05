/*
 * ELF文件解析
 *
 * 获取ELF文件各段的地址
 * 为加载到内存做准备
 * 
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef ELF_H
#define ELF_H

#include "std_type_define.h"

typedef uint16_t elf_half;		//半字
typedef uint32_t elf_word;		//字
typedef uint32_t elf_addr;		//地址
typedef uint32_t elf_off;		//偏移

/* elf 文件头 */
typedef struct
{
  uint8_t	elf_ident[16];		/* 魔数和一些信息 */
  elf_half	elf_type;			/* ELF文件类型 */
  elf_half	elf_machine;		/* 体系结构 */
  elf_word	elf_version;		/* ELF版本号 */
  elf_addr	elf_entry;			/* 虚拟地址 */
  elf_off	elf_phoff;			/* 程序头表在文件中的偏移 */
  elf_off	elf_shoff;			/* 段表在文件中的偏移 */
  elf_word	elf_flags;			/* ELF标志位 */
  elf_half	elf_ehsize;			/* ELF头文件本身大小 */
  elf_half	elf_phentsize;		/* 程序头表条目大小 */
  elf_half	elf_phnum;			/* 程序头表计数 */
  elf_half	elf_shentsize;		/* 段表条目大小 */
  elf_half	elf_shnum;			/* 段表条目数量 */
  elf_half	elf_shstrndx;		/* 字符串表 */
} elf_header;


/* 段头 */
typedef struct
{
  elf_word	seg_type;		/* 段类型 */
  elf_off	seg_offset;		/* 段文件偏移 */
  elf_addr	seg_vaddr;		/* 段虚拟地址 */
  elf_addr	seg_paddr;		/* 段物理地址 */
  elf_word	seg_filesz;		/* 段大小，在文件中 .bss段特殊 */
  elf_word	seg_memsz;		/* 段大小，在内存中 .bss段特殊 */
  elf_word	seg_flags;		/* 段标志 */
  elf_word	seg_align;		/* 段对齐 */
} elf_seg_header;


/* 节头  */
typedef struct
{
  elf_word	sec_name;		/* Section name (string tbl index) */
  elf_word	sec_type;		/* Section type */
  elf_word	sec_flags;		/* Section flags */
  elf_addr	sec_addr;		/* Section virtual addr at execution */
  elf_off	sec_offset;		/* Section file offset */
  elf_word	sec_size;		/* Section size in bytes */
  elf_word	sec_link;		/* Link to another section */
  elf_word	sec_info;		/* Additional section information */
  elf_word	sec_addralign;	/* Section alignment */
  elf_word	sec_entsize;	/* Entry size if section holds table */
} elf_sec_header;



#endif
