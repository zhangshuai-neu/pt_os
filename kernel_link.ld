ENTRY(main)
SECTIONS{
	.text 0x200000:
    {
    	/*内核启动代码*/
    	./kernel/main.o(.text)
    	
		/*系统调用代码|系统函数代码(非中断触发，内核直接使用)*/
		./system_call/system_call.o(.text)
    }
    
    /*未使用0x260000 ~ 0x2c0s000*/
    

    .data 0x2e0000:
    {
        ./kernel/main.o(.data)
        ./system_call/system_call.o(.data)
    }
    
    .bss 0x2f0000:
    {
        ./kernel/main.o(.bss)
        ./system_call/system_call.o(.bss)
    }
    
}
