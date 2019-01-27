/*
notice:
	1) .text和.data等段之后必须有一个空格，否则会报语法错误
*/

ENTRY(main)
SECTIONS{
    /*从32M开始*/
	.text 0x2000000:
    {
    	*.o(.text)
    }
    /*从32M+4K开始*/
    .data 0x2001000:
    {
        *.o(.data)
    }
    .bss :
    {
        *.o(.bss)
    }
}
