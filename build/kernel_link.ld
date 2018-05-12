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
    
    .data :
    {
        ./build/main.o(.data)
        ./build/system_call.o(.data)
    }
    
}
