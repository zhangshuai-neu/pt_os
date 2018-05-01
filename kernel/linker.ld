ENTRY(main)
SECTIONS{
    .text 0x0:
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
