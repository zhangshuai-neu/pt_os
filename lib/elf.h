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

#define IDENT_NUM ((uint32_t)16)

/* elf 文件头 */
typedef struct
{
  uint8_t	elf_ident[IDENT_NUM];	/* 魔数和一些信息 */
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

//segment type 段类型
#define	SEG_T_NULL		0				/* 由程序头表入口使用 */
#define SEG_T_LOAD		1				/* 加载程序段 */
#define SEG_T_DYNAMIC	2				/* 动态链接信息 */
#define SEG_T_INTERP	3				/* 程序解释器 */
#define SEG_T_NOTE		4				/* 辅助信息 */
#define SEG_T_SHLIB		5				/* 保留 */
#define SEG_T_PHDR		6				/* 自己在头表中的入口 */
#define SEG_T_TLS		7				/* 本地线程存储段 */
#define	SEG_T_NUM		8				/* 定义类型数 */

#define SEG_T_LOOS		0x60000000		/* SEG_Tart of OS-specific */
#define SEG_T_GNU_EH_FRAME	0x6474e550	/* GCC .eh_frame_hdr segment */
#define SEG_T_GNU_SEG_TACK	0x6474e551	/* 表明堆栈是否可执行 */
#define SEG_T_GNU_RELRO	0x6474e552		/* 冲定位后只读Read-only after relocation */
#define SEG_T_LOSUNW	0x6ffffffa
#define SEG_T_SUNWBSS	0x6ffffffa		/* Sun Specific segment */
#define SEG_T_SUNWSEG_TACK	0x6ffffffb	/* SEG_Tack segment */
#define SEG_T_HISUNW	0x6fffffff
#define SEG_T_HIOS		0x6fffffff		/* End of OS-specific */
#define SEG_T_LOPROC	0x70000000		/* SEG_Tart of processor-specific */
#define SEG_T_HIPROC	0x7fffffff		/* End of processor-specific */


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
