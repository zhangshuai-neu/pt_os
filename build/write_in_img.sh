#! /bin/bash
#-----------------------------
#各部分所占用扇区数
#-----------------------------
MBR_SECTOR_SIZE=1			#1

LOADER_SECTOR_START=1
LOADER_SECTOR_SIZE=17		#2-18

KERNEL_SECTOR_START=19		#19-219
KERNEL_SECTOR_SIZE=2048

USER=`who |head -1 |cut -d' ' -f1`
BUILD_DIR=/home/${USER}/Desktop/pt_os/build

dd if=${BUILD_DIR}/mbr.bin 	 	of=./pt_os.img bs=512 count=${MBR_SECTOR_SIZE} conv=notrunc
dd if=${BUILD_DIR}/loader.bin   of=./pt_os.img bs=512 count=${LOADER_SECTOR_SIZE} seek=${LOADER_SECTOR_START} conv=notrunc
dd if=${BUILD_DIR}/kernel.elf   skip=8 of=./pt_os.img bs=512 count=${KERNEL_SECTOR_SIZE} seek=${KERNEL_SECTOR_START} conv=notrunc
