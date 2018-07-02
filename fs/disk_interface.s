[bits 32]

extern rd_disk_m_32

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
