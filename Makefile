#-----------------------------
#各部分所占用扇区数
#-----------------------------
MBR_SECTOR_SIZE:=1

LOADER_SECTOR_START:=1
LOADER_SECTOR_SIZE:=17

KERNEL_SECTOR_START:=18
KERNEL_SECTOR_SIZE:=200
#kernel不超过512KB

run: boot_code kernel_code
	/home/shuai_zhang/Desktop/bochs-2.6.9/bochs -f bochsrc
	
boot_code:./boot/1mbr.bin ./boot/2loader.bin
	$(MAKE) -C ./boot
	dd if=./boot/1mbr.bin of=./pt_os.img bs=512 count=$(MBR_SECTOR_SIZE) \	
	conv=notrunc
	dd if=./boot/2loader.bin of=./pt_os.img bs=512 count=$(LOADER_SECTOR_SIZE) \
	seek=$(LOADER_SECTOR_START) conv=notrunc
	
kernel_code:./kernel/kernel.bin
	$(MAKE) -C ./kernel
	dd if=./kernel/kernel.bin skip=2000 of=./pt_os.img bs=512 \
	count=$(KERNEL_SECTOR_SIZE) seek=$(KERNEL_SECTOR_START) conv=notrunc

#使用bochs自带的镜像制作程序
image:
	/home/shuai_zhang/Desktop/bochs-2.6.9/bximage
