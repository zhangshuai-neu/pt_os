ENTRY(main)
SECTIONS{
    .text 0x100000:
    {
		main.o(.text)
		
        *(.text)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}
