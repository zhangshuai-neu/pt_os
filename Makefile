#编译器、链接器
CC = gcc
LD = ld

#目录
USER=$(shell who |head -1 |cut -d' ' -f1)
BUILD_DIR = ./build

#编译选项
LIB = -I sc -I lib -I it
C_FLAGS = -Wall -m32 -c $(LIB) 

#链接选项
LD_OBJECTS= $(BUILD_DIR)/main.o $(BUILD_DIR)/system_call.o
LD_FLAGS:=-m elf_i386 -T $(BUILD_DIR)/kernel_link.ld 

#汇编代码
asm: boot/mbr.S boot/loader.S boot/boot_parameter.inc
	#启动
	nasm -I boot/ -o $(BUILD_DIR)/mbr.bin boot/mbr.S
	nasm -I boot/ -o $(BUILD_DIR)/loader.bin boot/loader.S 
	#中断

#模块
$(BUILD_DIR)/main.o: kernel/main.c sc/system_call.h
	$(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/system_call.o: sc/system_call.c sc/system_call.h lib/stdint.h
	$(CC) $(C_FLAGS) $< -o $@
	
build: asm $(LD_OBJECTS)
	ld $(LD_FLAGS) $(LD_OBJECTS) -o $(BUILD_DIR)/kernel.elf

#写入disk
hd:
	./build/write_in_img.sh

#bochs运行
run:
	sed -i s/USER/${USER}/g  bochs_conf/bochsrc
	/home/$(USER)/Desktop/bochs-2.6.9/bochs -f bochs_conf/bochsrc

#清空所有.o
clean:
	rm $(LD_OBJECTS) $(BUILD_DIR)/kernel.elf
	rm boot/*.bin

#使用bochs自带的镜像制作程序
image:
	/home/$(USER)/Desktop/bochs-2.6.9/bximage


