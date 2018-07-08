/*
notice:
	1) .text和.data等段之后必须有一个空格，否则会报语法错误
*/

ENTRY(main)
SECTIONS{

	.text 0x200000:
    {
    	/*内核启动代码*/
    	./build/main.o(.text)
    	 . = ALIGN(4);
		
		/*系统调用代码|系统函数代码(非中断触发，内核直接使用)*/
		./build/system_call.o(.text)
		 . = ALIGN(4);
        
        /*中断*/
        ./build/interrupt.o(.text)
         . = ALIGN(4);
        ./build/asm_it.o(.text)
        
        /*硬盘接口*/
        ./build/disk_interface.o(.text)
        
    }
    
    .data :
    {
        ./build/main.o(.data)
         . = ALIGN(4);
        
        ./build/system_call.o(.data)
         . = ALIGN(4);
        
        ./build/interrupt.o(.data)
         . = ALIGN(4);
        ./build/asm_it.o(.data)
        
        ./build/disk_interface.o(.data)
        
    }
    
}
