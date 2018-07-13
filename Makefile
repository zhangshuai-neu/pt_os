#notice:
# ifeq 之前不能为tab，只能用空格

#自己修改
MM_VERSION=1

#编译器、链接器
CC = gcc
LD = ld
AS = nasm

#目录
USER=$(shell who |head -1 |cut -d' ' -f1)
BUILD_DIR = ./build

#编译选项
LIB = -I sc -I lib -I it -I dev -I kernel -I fs -I mm_v$(MM_VERSION)
C_FLAGS = -Wall -m32 -c -fno-zero-initialized-in-bss -fno-stack-protector \
			$(LIB) -nostdinc

#链接选项
LD_OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/system_call.o \
			$(BUILD_DIR)/asm_it.o $(BUILD_DIR)/interrupt.o \
			$(BUILD_DIR)/timer.o $(BUILD_DIR)/disk_interface.o \
			$(BUILD_DIR)/mm.o
			
LD_FLAGS = -m elf_i386 -T $(BUILD_DIR)/kernel_link.ld 

#汇编代码
asm: boot/mbr.S boot/loader.S boot/boot_parameter.inc
	#-----------------------编译汇编代码-start-------------------------
	#启动
	$(AS) -I boot/ -o $(BUILD_DIR)/mbr.bin boot/mbr.S
	$(AS) -I boot/ -o $(BUILD_DIR)/loader.bin boot/loader.S 
	#中断
	$(AS) -f elf32 -o $(BUILD_DIR)/asm_it.o it/interrupt.s
	#-----------------------编译汇编代码-end-------------------------
	#

#模块
$(BUILD_DIR)/main.o: kernel/main.c
	$(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/system_call.o: sc/system_call.c sc/system_call.h
	$(CC) $(C_FLAGS) $< -o $@
	
$(BUILD_DIR)/interrupt.o: it/interrupt.c it/interrupt.h 
	$(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/timer.o: dev/timer.c dev/timer.h
	$(CC) $(C_FLAGS) $< -o $@
    
$(BUILD_DIR)/disk_interface.o: fs/disk_interface.c fs/disk_interface.h
	$(CC) $(C_FLAGS) $< -o $@


$(BUILD_DIR)/mm.o: mm_v$(MM_VERSION)/mm.c  mm_v$(MM_VERSION)/mm.h
	$(CC) $(C_FLAGS) $< -o $@

build: asm $(LD_OBJECTS)
	#-----------------------编译链接-start-------------------------------
	ld $(LD_FLAGS) $(LD_OBJECTS) -o $(BUILD_DIR)/kernel.elf
	#-----------------------编译阶段-end---------------------------------
	#

#写入disk
hd:
	#-----------------------写入硬盘-start-------------------------------
	./build/write_in_img.sh
	#-----------------------写入硬盘-end---------------------------------
	#

#bochs运行
run:
	#-----------------------启动bochs运行--------------------------------
	sed  s/USER/${USER}/g  bochs_conf/bochsrc > bochs_conf/${USER}_bochsrc
	/home/$(USER)/Desktop/bochs-2.6.9/bochs -f bochs_conf/${USER}_bochsrc

all: build hd run

#清空所有.o
clean:
	rm $(LD_OBJECTS) $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/*.bin
	
#使用bochs自带的镜像制作程序
image:
	/home/$(USER)/Desktop/bochs-2.6.9/bximage


