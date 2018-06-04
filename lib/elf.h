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

#define EI_NIDENT (16)

typedef uint16_t Elf32_Half;		//Type for a 16-bit quantity
typedef uint32_t Elf32_Word;		//Types for signed and unsigned 32-bit quantities
typedef uint64_t Elf32_Xword;		//Types for signed and unsigned 64-bit quantities
typedef uint32_t Elf32_Addr;		//Type of addresses
typedef uint32_t Elf32_Off;			//Type of file offsets
typedef uint16_t Elf32_Section;		//Type for section indices, which are 16-bit quantities
typedef Elf32_Half Elf32_Versym;	//Type for version symbol information

typedef struct
{
  unsigned char	e_ident[EI_NIDENT];		/* 魔数和一些信息 */
  Elf32_Half	e_type;					/* ELF文件类型 */
  Elf32_Half	e_machine;				/* 体系结构 */
  Elf32_Word	e_version;				/* ELF版本号 */
  Elf32_Addr	e_entry;				/* 虚拟地址 */
  Elf32_Off		e_phoff;				/* 程序头表在文件中的偏移 */
  Elf32_Off		e_shoff;				/* 段表在文件中的偏移 */
  Elf32_Word	e_flags;				/* ELF标志位 */
  Elf32_Half	e_ehsize;				/* ELF头文件本身大小 */
  Elf32_Half	e_phentsize;			/* 程序头表条目大小 */
  Elf32_Half	e_phnum;				/* 程序头表计数 */
  Elf32_Half	e_shentsize;			/* 段表条目大小 */
  Elf32_Half	e_shnum;				/* 段表条目数量 */
  Elf32_Half	e_shstrndx;				/* 字符串表 */
} Elf32_Ehdr;




#endif
