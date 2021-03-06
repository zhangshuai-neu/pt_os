#notice:
# ifeq 之前不能为tab，只能用空格

#自己修改
MM_VERSION=1
TM_VERSION=1

#编译器、链接器
CC = gcc
LD = ld
AS = nasm

#目录
USER=$(shell who |head -1 |cut -d' ' -f1)
BUILD_DIR = ./build

#编译选项
LIB = -I sc -I lib -I it -I dev -I kernel -I fs -I mm_v$(MM_VERSION) \
	-I tm_v$(MM_VERSION)
C_FLAGS = -Wall -m32 -c -fno-zero-initialized-in-bss -fno-stack-protector \
			$(LIB) -nostdinc

#链接选项
LD_OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/system_call.o \
			$(BUILD_DIR)/asm_it.o $(BUILD_DIR)/interrupt.o \
			$(BUILD_DIR)/timer.o $(BUILD_DIR)/disk_interface.o \
			$(BUILD_DIR)/mm.o $(BUILD_DIR)/lib.a \
			$(BUILD_DIR)/switch.o $(BUILD_DIR)/tm.o

LD_FLAGS = -m elf_i386 -T $(BUILD_DIR)/kernel_link.ld


#汇编代码
asm: boot/mbr.S boot/loader.S boot/boot_parameter.inc tm_v$(MM_VERSION)/switch.S
	@ $(AS) -I boot/ -o $(BUILD_DIR)/mbr.bin boot/mbr.S
	@ $(AS) -I boot/ -o $(BUILD_DIR)/loader.bin boot/loader.S
	@ $(AS) -f elf32 -o $(BUILD_DIR)/asm_it.o it/interrupt.s
	@ $(AS) -f elf32 -o $(BUILD_DIR)/switch.o tm_v$(MM_VERSION)/switch.S

#模块
$(BUILD_DIR)/main.o: kernel/main.c
	@ $(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/system_call.o: sc/system_call.c sc/system_call.h
	@ $(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/interrupt.o: it/interrupt.c it/interrupt.h
	@ $(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/timer.o: dev/timer.c dev/timer.h
	@ $(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/disk_interface.o: fs/disk_interface.c fs/disk_interface.h
	@ $(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/mm.o: mm_v$(MM_VERSION)/mm.c  mm_v$(MM_VERSION)/mm.h
	@ $(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/tm.o: tm_v$(TM_VERSION)/tm.c  tm_v$(TM_VERSION)/tm.h
	@ $(CC) $(C_FLAGS) $< -o $@

#静态库
$(BUILD_DIR)/lib.a: $(BUILD_DIR)/bitmap.o $(BUILD_DIR)/list.o
	@ ar crv $(BUILD_DIR)/lib.a $^

$(BUILD_DIR)/bitmap.o: lib/bitmap.c
	@ $(CC) $(C_FLAGS) $^ -o $@

$(BUILD_DIR)/list.o: lib/list.c
	@ $(CC) $(C_FLAGS) $^ -o $@

build: asm $(LD_OBJECTS)
	@ ld $(LD_FLAGS) $(LD_OBJECTS) -o $(BUILD_DIR)/kernel.elf

#写入disk
hd:
	./build/write_in_img.sh

#bochs运行
run:
	#-----------------------启动bochs运行--------------------------------
	sed  s/USER/${USER}/g  bochs_conf/bochsrc > bochs_conf/${USER}_bochsrc
	/home/$(USER)/Desktop/bochs-2.6.9/bochs -f bochs_conf/${USER}_bochsrc

all: asm build hd run

#清空所有.o
clean:
	rm $(LD_OBJECTS) $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/*.bin

#使用bochs自带的镜像制作程序
image:
	/home/$(USER)/Desktop/bochs-2.6.9/bximage
