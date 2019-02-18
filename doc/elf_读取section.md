## ELF文件格式读取指定section的内容

Linux直接给出了elf的说明，可以在 /usr/include/elf.h 中查看定义（如果你感兴趣）。

但是，直接看代码是不容易看明白的，有一些细微之处需要说明。在此整理：

#### 1） elf_header 简要说明
rong
生成的elf文件都有elf文件头，在整个文件的开头

可以读文件打印出来，和readelf命令的结果对比。(例如: readelf -h /bin/ls)

看一下magic number就知道对不对了。

```
/* elf 文件头 */
struct elf_header
{
    uint8_t	elf_ident[IDENT_NUM];   /* 魔数和一些信息 */
    elf_half	elf_type;           /* ELF文件类型 */
    elf_half	elf_machine;        /* 体系结构 */
    elf_word	elf_version;        /* ELF版本号 */
    elf_addr	elf_entry;          /* 虚拟地址 */
    elf_off     elf_phoff;          /* 程序头表在文件中的偏移 */
    elf_off     elf_shoff;          /* 段表在文件中的偏移 */
    elf_word	elf_flags;          /* ELF标志位 */
    elf_half	elf_ehsize;         /* ELF头文件本身大小 */
    elf_half	elf_phentsize;      /* 程序头表条目大小 */
    elf_half	elf_phnum;          /* 程序头表计数 */
    elf_half	elf_shentsize;      /* 段表条目大小 */
    elf_half	elf_shnum;          /* 段表条目数量 */
    elf_half	elf_shstrndx;       /* 字符串表，是shstrtab section的索引号*/
};
```

#### 2） elf_section_header 简要说明

ELF文件格式有很多的segment，每个segment又包括很多section。

最常见的section就是.text, .data 和 .bss 这三个。

.text存放了代码，.data存放了数据，.bss存放了尚未分配和初始化的全局变量等内容。

另外还有一个比较重要的section  —— **.shstrtab**,这个section存放了section的名字。

因为我们要读取指定section的内容，所以需要从 **.shstrtab** 获取section的名字加以判断。

```
/* section 段 */
struct elf_section_header
{
    // 用来指明 shsymtabl 中section name的位置，
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
```

#### 3） 根据section名字找到指定section