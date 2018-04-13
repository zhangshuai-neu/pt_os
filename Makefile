LOADER_SECTOR_SIZE:=17

boot_code:
	$(MAKE) -C ./boot
	dd if=./boot/1mbr.bin 	 of=./zs_os.img bs=512 count=1  conv=notrunc
	dd if=./boot/2loader.bin of=./zs_os.img bs=512 count=$(LOADER_SECTOR_SIZE) seek=1 conv=notrunc
run:
	/home/zs/Desktop/bochs-2.6.9/bochs -f bochsrc
