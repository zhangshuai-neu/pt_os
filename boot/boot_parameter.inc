;--------------------------MBR程序：读取信息------------------------------------
MBR_MEM_BASE_ADDR 			equ  0x7c00 	;MBR的内存起始地址，cs:ip = 0000:7c00
MBR_DISK_BASE_SECTOR 		equ  0x0 		;MBR起始扇区
;结束内存地址LOADER_MEM_BASE_ADDR-1

;--------------------------loader程序:(进入保护模式、内存大小、开启页式管理)----
LOADER_MEM_BASE_ADDR 		equ  0x7e00 	;loader程序其实内存地址设置为0x7e00, cs:ip = 0000:7e00
LOADER_DISK_BASE_SECTOR 	equ  0x1 		;loader程序从硬盘的1扇区开始（0扇区为MBR）
LOADER_SECTOR_SIZE 			equ  0x9 		;loader程序的大小（扇区数）
VGA_BASE_ADDR 				equ  0xb800 	;显示内存位置,段基址

;结束内存位置GDTR_BASE_ADDR-1

;--------------------------loader程序: gdt描述符  ------------------------------
DESC_NUM 		  equ 		4						;自己添加的描述符表项个数，系统段
GDTR_BASE_ADDR 	  equ 		0x00009000 				;GDT的基址，需要存放到GDTR中，32位
													;希望GDT在0x9000到0xa000之间，一共有512个段描述符，支持512-4个进程
GDTR_END_ADDR	  equ		0x0000a000				;GDT表的末尾
GDT_SECTOR_SIZE	  equ		0x8						;GDT表的大小
GDTR_LIMIT 		  equ 		(DESC_NUM*8-1)			;GDT系统段上限，字节为单位，16位
;结束内存地址GDTR_END_ADDR-1

;--------------------------loader程序: 内存大小记录-----------------------------
MEM_SIZE_RECORD_ADDR			equ 	0xa000				;记录内存大小的一段内存空间，起始地址
MEM_SIZE_END_ADDR				equ		0xa200				;记录内存大小的一段内存空间，结束地址
MEM_SIZE_AVAI_ADDR				equ		(MEM_SIZE_END_ADDR-8)	;可用内存大小，存放到该位置

;--------------------------MBR程序: mbr要读取的总扇区数-------------------------
MBR_READ_SECTOR_SIZE	equ		(LOADER_SECTOR_SIZE+GDT_SECTOR_SIZE)

;--------------------------loader程序：页表-----------------------------------
PAGE_DIR_BASE_ADDR		equ		0x0010_0000	;页表所在的基地址(1M~2M)

;--------------------------kernel程序：内核代码--------------------------------
KERNEL_BASE_ADDR	equ		0x0020_0000		;内核加载到内存的的位置(2M~3M)
KERNEL_ENTRY_POINT	equ 	0x0020_0000		;可能会改变
KERNEL_BIN_BASE_SECTOR	equ		19			;kernel.bin的起始扇区号
KERNEL_BIN_SECTOR_SIZE	equ		200			;kernel.bin的最大长度（1M 4096）

;--------------------------loader程序：loader要读取的扇区数---------------------
LOADER_READ_SECTOR_SIZE	equ		KERNEL_BIN_SECTOR_SIZE

;GDT表项的属性
DESC_G_4K   equ	  1_000_0000_0000_0000_0000_0000b   ;limit的单位为4K
DESC_D_32   equ	   1_00_0000_0000_0000_0000_0000b	;置1,表示使用EIP和ESP（而不是ip和sp）
DESC_L	    equ	    0_0_0000_0000_0000_0000_0000b	;64位代码标记，此处标记为0便可。
DESC_AVL    equ	      0_0000_0000_0000_0000_0000b	;cpu不用此位，暂置为0  
DESC_LIMIT_CODE2  equ   1111_0000_0000_0000_0000b	;代码段上限
DESC_LIMIT_DATA2  equ	         DESC_LIMIT_CODE2 	;数据段上限
DESC_LIMIT_VIDEO2 equ   0000_0000_0000_0000_0000b	;显示段上限
DESC_P	    equ		  	    1_000_0000_0000_0000b	;present，表示此段是否在内存中，为0停止读取
DESC_DPL_0  equ		         00_0_0000_0000_0000b 	;特权级别
DESC_DPL_1  equ		         01_0_0000_0000_0000b
DESC_DPL_2  equ		         10_0_0000_0000_0000b
DESC_DPL_3  equ		         11_0_0000_0000_0000b	
DESC_S_CODE equ		            1_0000_0000_0000b 	;非系统段（1为非系统段，0为系统段）
DESC_S_DATA equ	  				    DESC_S_CODE 	;非系统段
DESC_S_sys  equ		    	    0_0000_0000_0000b 	;系统段
DESC_TYPE_CODE  equ	     	      1000_0000_0000b	;x=1,c=0,r=0,a=0 代码段是可执行的,非依从的,不可读的,已访问位a清0.  
DESC_TYPE_DATA  equ	    	      0010_0000_0000b	;x=0,e=0,w=1,a=0 数据段是不可执行的,向上扩展的,可写的,已访问位a清0.

;gdt第一段，默认为0
DESC_FIRST_LOW4  equ 0x00000000
DESC_FIRST_HIGH4 equ 0x00000000

;代码段，段描述符，段基址为0
DESC_CODE_LOW4  equ  0x0000FFFF
DESC_CODE_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_CODE2 + DESC_P + DESC_DPL_0 + DESC_S_CODE + DESC_TYPE_CODE + 0x00

;数据段，段描述符，段基址为0  堆栈段也是该段
DESC_DATA_LOW4  equ  0x0000FFFF
DESC_DATA_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_DATA2 + DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00

;显示段，段描述符，段基址为0xb8000
DESC_VIDEO_LOW4  equ 0x80000007
DESC_VIDEO_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_VIDEO2 + DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x0b


;---------------------------loader程序：段选择子属性---------------------------------
RPL0  	 equ   00b 		;最高权限
RPL1  	 equ   01b
RPL2  	 equ   10b
RPL3  	 equ   11b		;最低权限
TI_GDT	 equ   000b 	;访问GDT
TI_LDT	 equ   100b 	;访问LDT

;段选择子_代码段
SELECTOR_CODE  equ (0x0001<<3) + TI_GDT + RPL0

;段选择子_数据段
SELECTOR_DATA  equ (0x0002<<3) + TI_GDT + RPL0

;段选择子，堆栈段和数据段一致
STACK_TOP 	   equ  (MBR_MEM_BASE_ADDR-8)	;栈顶靠近0x7c00，x500到0x7c00为一段空余内存（近30KB）

;段选择子_显示段
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0

;--------------------------loader程序：页表相关属性----------------------
PG_P  equ   1b			;存在位（是否在物理内存）
PG_RW_R	 equ  00b 		;读写位，可读不可写
PG_RW_W	 equ  10b 		;读写位，可读可写
PG_US_S	 equ  000b 		;用户|特权位，特权
PG_US_U	 equ  100b 		;用户|特权位，用户

;---------------------------program type 定义   -----------------------
PT_NULL	 equ  0			;表示












