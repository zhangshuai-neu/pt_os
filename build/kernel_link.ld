/*
notice:
	1. .text和.data等段之后必须有一个空格，否则会报语法错误
*/

ENTRY(main)
SECTIONS{

	.text 0x200000:
    {
    	/*内核启动代码*/
    	./build/main.o(.text)
    	
		/*系统调用代码|系统函数代码(非中断触发，内核直接使用)*/
		./build/system_call.o(.text)
    }
    /*
    所有全局变量不能初始化为0，因为会被放到.bss段，
    内核启动我没有写elf解释程序，所有不能为.bss段动态分配内存，
 	这会导致严重错误。到用户程序时，该问题会解决
    */
    .data :
    {
        ./build/main.o(.data)
        ./build/system_call.o(.data)
    }
    
}
