all:
	gcc example.c -m32 -c -o example.o
	ld example.o -m elf_i386 -T user_prog.ld -o example.elf
	gcc test.c ../elf.c -m32 -o test

clean:
	rm test
