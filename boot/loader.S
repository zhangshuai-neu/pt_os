%include "boot_parameter.inc" ;一些有关参数
section loader vstart=LOADER_MEM_BASE_ADDR ;部分工作在实模式,然后进入保护模式
show_msg:			
	;清屏
	mov ax,0x600 	;ah=0x6,向上滚屏幕;ah=0x07，向下滚屏
	mov bx,0x700	;
	mov cx,0 		;ch,cl对应左上角(x,y)
	mov dx,0x184f 	;dh,dl对应右下角(x,y)，25×80
	int 0x10			
					;获得光标位置
	mov ah, 3		;输入: 3号子功能是获取光标位置,需要存入ah寄存器
	mov bh, 0		;bh寄存器存储的是待获取光标的页号
	int 0x10		;输出: ch=光标开始行,cl=光标结束行

	;打印msg欢迎信息
		mov ax,0 		;al存放ascii码字符
		mov dx,0x0f00 	;设置背景和字体颜色
		mov bx,msg 		;字符串基地址
		mov bp,0 		;显存地址偏移
	go_on_printing: 	;打印字符串
		mov byte al,[bx]
		cmp al,0 				;不等于0打印，等于0则结束
		jz show_end 			;到达字符串末尾
		mov dl,al				;复制字符
		mov word [gs:bp],dx 	;复制到显存		
		add bx,1 				;字符后移1位
		add bp,2 				;显存偏移加2
		jmp go_on_printing
	show_end: 		;打印结束
		jmp get_memory_size		;获取内存大小

define_data:
	msg db "welcome to pt_os" 	;启动欢迎信息
		db 0					;表示字符串的末尾


;-------------------获取内存大小------------------------------------------
;要在进入保护模式之前运行
;内存大小测量代码，使用BIOS中断的0x15的子功能0xe820,读取内存大小(网上查阅)
;--------------------------------------------------------------------------
get_memory_size:
	ards_num dw	0						;用来存放ards的个数
	jmp	get_mem_size					;跳转到读取内存大小的代码
	
	get_mem_size_err:					;内存大小读取错误会停止在这里
		mov byte [gs:160],'m'
		mov byte [gs:162],'e'
		mov byte [gs:164],'m'
		mov byte [gs:166],' '
		mov byte [gs:168],'e'
		mov byte [gs:170],'r'
		mov byte [gs:172],'r'
		jmp $							;停止执行
		
		;调用前配置寄存器
	get_mem_size:
		;调用前配置寄存器
		xor ebx,ebx						;ebx置0，第一次需设为0。以后每次调用返回后，BIOS会自动更新该值
		mov	edx,0x534d4150				;'SMAP'的ASCII码，一个固定的签名
		mov	di,MEM_SIZE_RECORD_ADDR		;BIOS会将ARDS存放到es:di指向的内存位置			
	loop1:
		mov eax,0x0000e820				;功能号
		mov ecx,20						;ARDS(address range descriptor structure)的字节大小（该中断功能的返回值）
		int 0x15
		
		;出错
		jc	get_mem_size_err			;该中断错误会将CF位置1
		;正确
		inc word [ards_num]				;记录ARDS的数量，inc指令(+1)
		add di,cx						;di增加20，指向新的ARDS结构位置
		cmp ebx,0						;若ebx==0，且cf不为1,说明ards全部返回
		jnz loop1						;读取下一个ards
			
	;找到可用内存，内存大小存放到MEM_SIZE_AVAI_ADDR指向的位置
		mov cx,[ards_num]				;ARDS的数量为循环次数
		mov ebx,MEM_SIZE_RECORD_ADDR	;ARDS记录的基址
		xor edx,edx						;edx存放内存块最大地址上限，起始最大值为0
	find_max_area:		;不用判断type是否为1,地址最高的内存块一定是可被使用的（也就是主板上的内存容量）
		mov eax,[ebx]	;[ebx]为base_addr_low，只需要低32bit即可
		add	eax,[ebx+8]	;[ebx+8]为length_low，只需要低32bit即可，和base_addr_low相加得到上限
		add ebx,20		;指向下块ARDS
		cmp	edx,eax		;比较eax和最大地址块上限
		jge	next_ards	;当前内存块比较小，则结束本次循环
		mov edx,eax		
	next_ards:
		loop find_max_area
	mem_get_ok:
		mov [MEM_SIZE_AVAI_ADDR],edx	;将内存容量，写入MEM_SIZE_AVAI_ADDR指向的内存
		mov byte [gs:160],'m'
		mov byte [gs:162],'e'
		mov byte [gs:164],'m'
		
		jmp into_pro	;进入保护模式


;---------------------进入保护模式-------------------------
;1 填充并加载gdt
;2 打开A20,关闭地址回绕
;3 将cr0，pe位置1 (CR控制寄存器),打开保护模式
;----------------------------------------------------------
into_pro:
	;1 gdt的内容已被载入内存，直接设置gdtr即可
	gdt_ptr:	dw GDTR_LIMIT 
				dd GDTR_BASE_ADDR
	mov bx,gdt_ptr
	lgdt [bx]
	
	;2 打开A20
	in al,0x92
	or al,0000_0010B
	out 0x92,al

	;3 cr0第0位置1  ----------------
	mov eax,cr0
	or  eax,0x00000001
	mov cr0,eax

	jmp dword SELECTOR_CODE:protection_mode_start 	;刷新流水线，跳转到保护模式执行
													;会将cs自动切换到SELECTOR_CODE

[bits 32]
protection_mode_start:
	mov ax,SELECTOR_DATA 	;设置额段选择子
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov ss,ax
	mov esp,STACK_TOP		;设置栈顶
	mov ax, SELECTOR_VIDEO
	mov gs, ax				;新的显示段

	;进入保护模式，寄存器也设置完成
	mov byte [gs:320],'p'
	mov byte [gs:322],' '
	mov byte [gs:324],'m'
	mov byte [gs:326],'o'
	mov byte [gs:328],'d'
	mov byte [gs:330],'e'

;----------------读取kernel.bin-------------------------
;loader程序读取内核代码
;-------------------------------------------------------
load_kernel_bin:
	mov eax, KERNEL_BIN_BASE_SECTOR
	mov ebx, KERNEL_BASE_ADDR 			;内存地址
	mov ecx, KERNEL_BIN_SECTOR_SIZE
	call rd_disk_m_32
	
	;debug
	

	
;-------------启用内存分页机制，页式管理内存------------
;采用 linux一样的做法，将0-3G作为虚拟的用户空间，3-4G位虚拟的系统空间
;物理上，0-2M作为实际使用系统空间;其他部分，由进程使用
;
;1 创建页目录和页表,初始化内存位图
;2 修改GDT
;3 页目录地址写入cr3，打开cr0的pg位
;4 用新地址重新加载GDT
;-------------------------------------------------------		
	;1 创建页目录和页表,初始化内存位图
	;初始化的页表大小为1M
	call setup_page
	
	;2 修改GDT
	sgdt [gdt_ptr]	;
	
	mov ebx,GDTR_BASE_ADDR
	or dword [ebx + 24 + 4],0xc0000000	;修改显示段
	
	add dword [gdt_ptr + 2],0xc0000000	;由于采用页式管理，访问GDT的地址也发生变化
	add esp,0xc0000000					;将堆栈空间指向系统内存
	
	
	;3 页目录地址写入cr3，打开cr0的pg位
	mov eax,PAGE_DIR_BASE_ADDR
	mov cr3,eax
	
	mov eax,cr0
	or	eax,0x80000000
	mov cr0,eax			;打开pg标志位
						;cs,ds,gs have changed to
	;4 用新地址重新加载GDT
	lgdt [gdt_ptr]

	mov byte [gs:480],'p'
	mov byte [gs:482],'a'
	mov byte [gs:484],'g'
	mov byte [gs:486],'e'

	jmp SELECTOR_CODE:enter_kernel	  ;强制刷新流水线,更新gdt

;----------------------进入内核-------------------------
;内核初始化
;-------------------------------------------------------	
enter_kernel:    
	mov esp, 0xc02b0000
	jmp KERNEL_ENTRY_POINT                 ;


;----------------一些用到的函数-------------------------
;1 rd_disk_m_32 功能：读取硬盘n个扇区（在32位模式） 
;2 mem_cpy 		功能：逐字节拷贝 mem_cpy(dst,src,size)
;3 setup_page 	功能：创建页目录及页表
;-----------------------------------------------------
;-------------------rd_disk_m_32 ---------------------  
rd_disk_m_32:	  
		; eax=LBA扇区号
		; ebx=将数据写入的内存地址
		; ecx=读入的扇区数
    mov esi,eax		;备份eax
    mov di,cx		;备份扇区数到di
;读写硬盘:
;第1步：设置要读取的扇区数
    mov dx,0x1f2
    mov al,cl
    out dx,al      ;读取的扇区数
    mov eax,esi	   ;恢复ax

;第2步：将LBA地址存入0x1f3 ~ 0x1f6
    ;LBA地址7~0位写入端口0x1f3
    mov dx,0x1f3                       
    out dx,al                          

    ;LBA地址15~8位写入端口0x1f4
    mov cl,8
    shr eax,cl
    mov dx,0x1f4
    out dx,al

    ;LBA地址23~16位写入端口0x1f5
    shr eax,cl
    mov dx,0x1f5
    out dx,al

    shr eax,cl
    and al,0x0f	   ;lba第24~27位
    or al,0xe0	   ;设置7～4位为1110,表示lba模式
    mov dx,0x1f6
    out dx,al

;第3步：向0x1f7端口写入读命令，0x20 
    mov dx,0x1f7
    mov al,0x20                        
    out dx,al

;至此,硬盘控制器便从指定的lba地址(eax)处,读出连续的cx个扇区,下面检查硬盘状态,不忙就能把这cx个扇区的数据读出来

;第4步：检测硬盘状态
  .not_ready:		   ;测试0x1f7端口(status寄存器)的的BSY位
    ;同一端口,写时表示写入命令字,读时表示读入硬盘状态
    nop
    in al,dx
    and al,0x88	   ;第4位为1表示硬盘控制器已准备好数据传输,第7位为1表示硬盘忙
    cmp al,0x08
    jnz .not_ready	   ;若未准备好,继续等。

;第5步：从0x1f0端口读数据
    mov ax, di	   ;以下从硬盘端口读数据用insw指令更快捷,不过尽可能多的演示命令使用,
	;在此先用这种方法,在后面内容会用到insw和outsw等

    mov dx, 256	   ;di为要读取的扇区数,一个扇区有512字节,每次读入一个字,共需di*512/2次,所以di*256
    mul dx
    mov cx, ax	   
    mov dx, 0x1f0
  .go_on_read:
    in ax,dx		
    mov [ebx], ax
    add ebx, 2
    loop .go_on_read
    ret


;-------------   创建页目录及页表   ---------------
; 使用了4KB*2大小的空间
; 起始地址	  功能
; 0x0010_0000 创建页目录
; 0x0010_1000 创建页表
setup_page:
	;先把页目录占用的空间逐字节清0
	mov ecx, 4096
	mov esi, 0
.clear_page_dir:
	mov byte [PAGE_DIR_BASE_ADDR + esi], 0
	inc esi
	loop .clear_page_dir

.create_pde:				     
	;创建页目录项(PDE)Page Directory Entry
	mov eax, PAGE_DIR_BASE_ADDR
	add eax, 0x1000 			     ;此时eax为第一个页表的位置及属性
	mov ebx, eax				     ;此处为ebx赋值，是为.create_pte做准备，ebx为基址。
	;下面将页目录项0和0xc00都存为第一个页表的地址，
	;一个页表可表示4MB内存,这样0xc03fffff以下的地址和0x003fffff以下的地址都指向相同的页表，
	;这是为将地址映射为内核地址做准备
	or	eax, PG_US_U | PG_RW_W | PG_P		;页目录项的属性RW和P位为1,US为1,表示用户属性,所有特权级别都可以访问.
	mov [PAGE_DIR_BASE_ADDR + 0x0], eax		;第1个目录项,在页目录表中的第1个目录项写入第一个页表的位置(0x101000)及属性(3)
	mov [PAGE_DIR_BASE_ADDR + 0xc00], eax	;一个页表项占用4字节,0xc00表示第768个页表占用的目录项,0xc00以上的目录项用于内核空间,
	;也就是页表的0xc0000000~0xffffffff共计1G属于内核,0x0~0xbfffffff共计3G属于用户进程.
	sub eax, 0x1000
	mov [PAGE_DIR_BASE_ADDR + 4092], eax	;使最后一个目录项指向页目录表自己的地址

;下面创建页表项(PTE)
	mov ecx, 1024				    	;4M低端内存 / 每页大小4k = 1024
	mov esi, 0
	mov edx, 0
	mov edx, PG_US_U | PG_RW_W | PG_P	;属性为7,US=1,RW=1,P=1
.create_pte:				     		;创建Page Table Entry
	mov [ebx+esi*4],edx			     	;此时的ebx已经在上面通过eax赋值为0x0010_1000,也就是第一个页表的地址 
	add edx,4096
	inc esi
	loop .create_pte

;创建内核其它页表的PDE
	mov eax, PAGE_DIR_BASE_ADDR
	add eax, 0x2000 		     		;此时eax为第二个页表的位置
	or	eax, PG_US_U | PG_RW_W | PG_P	;页目录项的属性RW和P位为1,US为0
	mov ebx, PAGE_DIR_BASE_ADDR
	mov ecx, 254			     		;范围为第769~1022的所有目录项数量
	mov esi, 769
.create_kernel_pde:
	mov [ebx+esi*4], eax
	inc esi
	add eax, 0x1000
	loop .create_kernel_pde
	ret
		
		
	times (GDTR_BASE_ADDR-LOADER_MEM_BASE_ADDR)-($-$$) db 0	;空余一段空间
;----------定义一些GDT的内容，会被加载到内存GDTR_BASE_ADDR------
section gdt vstart=GDTR_BASE_ADDR
	;GDT的填充值
	GDT_FIRST:
		dd DESC_FIRST_LOW4
		dd DESC_FIRST_HIGH4

	DESC_CODE:
		dd DESC_CODE_LOW4
		dd DESC_CODE_HIGH4
		
	DESC_DATA:
		dd DESC_DATA_LOW4
		dd DESC_DATA_HIGH4
		
	DESC_VIDEO:
		dd DESC_VIDEO_LOW4
		dd DESC_VIDEO_HIGH4

	;空余一段内容保存以后增加的GDT表项
	times (GDTR_END_ADDR-GDTR_BASE_ADDR)-($-$$) db 0
	
