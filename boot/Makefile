default:mbr.S loader.S boot_parameter.inc
	nasm -I include/ -o 1mbr.bin mbr.S
	nasm -I include/ -o 2loader.bin loader.S
clean:
	rm *.bin
