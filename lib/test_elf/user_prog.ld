/*
notice:
	1) .text和.data等段之后必须有一个空格，否则会报语法错误
*/

ENTRY(main)
SECTIONS{

	.text 0x200000:
    {
    	*.o(.text)
    }

    .data :
    {
        *.o(.data)
    }
}
