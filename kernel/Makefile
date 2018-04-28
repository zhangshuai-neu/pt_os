default:
	gcc -c -o main.o main.c	
	ld  -T linker.ld main.o -e main -o kernel.bin

clean:
	rm kernel.bin main.o
	
#ld链接代码
#-e 指明起始符号
#-T 指定链接脚本
#kernel.bin的内存起始位置(用readelf查看)
	
	

