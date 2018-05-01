#-----------------------------
#各部分所占用扇区数
#-----------------------------
MBR_SECTOR_SIZE:=1			#1

LOADER_SECTOR_START:=1
LOADER_SECTOR_SIZE:=17		#2-18

KERNEL_SECTOR_START:=19		#19-219
KERNEL_SECTOR_SIZE:=200
#kernel不超过512KB

all:
	#compile boot
	$(MAKE) -C ./boot
	dd if=./boot/1mbr.bin 	 of=./pt_os.img bs=512 count=$(MBR_SECTOR_SIZE) conv=notrunc
	dd if=./boot/2loader.bin of=./pt_os.img bs=512 count=$(LOADER_SECTOR_SIZE) seek=$(LOADER_SECTOR_START) conv=notrunc
	
	#compile kernel
	$(MAKE) -C ./kernel
	dd if=./kernel/kernel.bin skip=2048 of=./pt_os.img bs=512 count=$(KERNEL_SECTOR_SIZE) seek=$(KERNEL_SECTOR_START) conv=notrunc

run:
	/home/shuai_zhang/Desktop/bochs-2.6.9/bochs -f bochsrc


clean:
	$(MAKE) -C ./boot clean
	$(MAKE) -C ./kernel clean

#使用bochs自带的镜像制作程序
image:
	/home/shuai_zhang/Desktop/bochs-2.6.9/bximage