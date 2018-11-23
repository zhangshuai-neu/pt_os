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
        ./build/asm_it.o(.text)
         . = ALIGN(4);

        /*硬盘接口*/
        ./build/disk_interface.o(.text)
         . = ALIGN(4);

        /*内存管理*/
        ./build/mm.o(.text)
         . = ALIGN(4);

        /*任务管理*/
        ./build/switch.o(.text)
        ./build/tm.o(.text)
         . = ALIGN(4);

        /*内核使用的库文件*/
        ./build/lib.a(.text)
         . = ALIGN(4);
    }

    .data :
    {
        ./build/main.o(.data)

        ./build/system_call.o(.data)

        ./build/interrupt.o(.data)

        ./build/asm_it.o(.data)

        ./build/disk_interface.o(.data)

        ./build/mm.o(.data)

        ./build/switch.o(.data)
        ./build/tm.o(.data)

        ./build/lib.a(.data)
    }
}
