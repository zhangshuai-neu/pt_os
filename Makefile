
#kernel不超过1M

#ld链接代码
#-e 指明起始符号
#-T 指定链接脚本
#kernel.bin的内存起始位置(用readelf查看)
LD_OBJECTS:=./kernel/main.o ./system_call/system_call.o
LD_FLAGS:=-m elf_i386 -T kernel_link.ld -o kernel.elf


all:
	#boot
	$(MAKE) -C ./boot
	#kernel
	$(MAKE) -C ./kernel
	
	#system_call
	$(MAKE) -C ./system_call
	
	ld $(LD_FLAGS) $(LD_OBJECTS)
	
run:
	/home/shuai_zhang/Desktop/bochs-2.6.9/bochs -f bochsrc


clean:
	$(MAKE) -C ./boot clean
	$(MAKE) -C ./kernel clean
	$(MAKE) -C ./system_call clean

#使用bochs自带的镜像制作程序
image:
	/home/shuai_zhang/Desktop/bochs-2.6.9/bximage
