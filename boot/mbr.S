%include "boot_parameter.inc"

section MBR vstart=MBR_MEM_BASE_ADDR 	;工作在实模式，MBR
init_registers: ;初始化寄存器
	mov ax,cs 			;cs=0
	mov ds,ax 			;ds=0
	mov es,ax 			;es=0
	mov ss,ax 			;ss=0
	mov sp,STACK_TOP 	;实模式内存表中可知，0x500~0x7bff为空闲，可以作为堆栈
	mov ax,VGA_BASE_ADDR
	mov gs,ax 	  		;实模式内存表可知，0xb8000为显存基地址

read_disk:							;读写磁盘，LBA方式
	;1 设置要读取扇区数
	mov dx,0x1f2 					;外部端口号
	mov cx,MBR_READ_SECTOR_SIZE		;读取扇区数（低8位）
	mov al,cl 						;
	out dx,al 						;写入端口
	
	
	;2 设置LBA地址(描述扇区的位置)
	mov eax,LOADER_DISK_BASE_SECTOR	;读取起始扇区,1
	mov cl,8
	mov dx,0x1f3    ;LBA地址7~0位写入端口0x1f3       
    out dx,al                          
    shr eax,cl
    mov dx,0x1f4					
    out dx,al		;LBA地址15~8位写入端口0x1f4
    shr eax,cl
    mov dx,0x1f5					
    out dx,al		;LBA地址23~16位写入端口0x1f5
    shr eax,cl
    and al,0x0f		;lba第24~27位
    or al,0xe0		;设置7～4位为1110,表示lba模式
    mov dx,0x1f6
    out dx,al		;写入端口0x1f6

	;3 向0x1f7端口写入读命令，0x20,硬盘开始读取(设置完就自动读取，不需要bios中断服务)
	mov dx,0x1f7
	mov al,0x20                        
	out dx,al

	;4 检测硬盘状态
	not_ready:
		nop				;等待一点时间，让磁盘进行工作
		in al,dx		;
		and al,0x88		;第4位为1表示硬盘控制器已准备好数据传输，第7位为1表示硬盘忙
		cmp al,0x08
		jnz not_ready	;若未准备好，继续等
	
	;5 从0x1f0端口读数据
	mov ax,MBR_READ_SECTOR_SIZE
	mov dx,256
	mul dx			;结果存放在ax中
	mov cx,ax		;di为要读取的扇区数，一个扇区有512字节，每次读一个字，需di*256次
	mov dx, 0x1f0	;端口号
	mov bx,LOADER_MEM_BASE_ADDR	;读到内存的位置，0x0
	go_on_reading:
		in  ax,dx
		mov [bx],ax
		add bx,2				;内存地址后移
		loop go_on_reading		;循环次数为cx

end_mbr:
	jmp LOADER_MEM_BASE_ADDR	;跳转到loader程序

	times 510-($-$$) db 0		;空余空间set 0
	db 0x55,0xaa	;MBR标志
	
	
	
	
	
	
