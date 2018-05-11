#编译器、链接器
CC = gcc
LD = ld

#build目录
BUILD_DIR = ./build

#编译选项
LIB = -I sc -I lib -I it
C_FLAGS = -Wall -m32 -c $(LIB) 

#链接选项
LD_OBJECTS= $(BUILD_DIR)/main.o $(BUILD_DIR)/system_call.o
LD_FLAGS:=-m elf_i386 -T $(BUILD_DIR)/kernel_link.ld 

#启动代码
boot:
	$(MAKE) -C ./boot 

#编译各个模块
$(BUILD_DIR)/main.o: kernel/main.c sc/system_call.h
	$(CC) $(C_FLAGS) $< -o $@

$(BUILD_DIR)/system_call.o: sc/system_call.c sc/system_call.h lib/stdint.h
	$(CC) $(C_FLAGS) $< -o $@
	
build: $(LD_OBJECTS)
	ld $(LD_FLAGS) $(LD_OBJECTS) -o $(BUILD_DIR)/kernel.elf

#写入disk
hd:
	./build/write_in_img.sh

#bochs运行
run:
	/home/shuai_zhang/Desktop/bochs-2.6.9/bochs -f ./bochs_conf/bochsrc

#清空所有.o
clean:
	rm $(LD_OBJECTS) $(BUILD_DIR)/kernel.elf

#使用bochs自带的镜像制作程序
image:
	/home/shuai_zhang/Desktop/bochs-2.6.9/bximage


