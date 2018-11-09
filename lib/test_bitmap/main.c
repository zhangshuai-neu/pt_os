#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "bitmap.h"

int main(){
	struct bitmap bm;
	char * new_bits = (char *)malloc(128);
	bm.btmp_bytes_len = 128;
	bm.bits = new_bits;
	
    //初始化
    init_bitmap(&bm);
    
    //将bm的头8个bit置为1
    bitmap_set_cont_bits(&bm,0,8,1);
	
    //将bm的4~7的bit置为0
    bitmap_set_cont_bits(&bm,4,4,0);
    
    //第0个bit被置为1,则返回1
    printf("bitmap_verify_bitset(&bm,0): %d\n",bitmap_verify_bitset(&bm,0));    
    //第8个bit没有被置为1,则返回0
    printf("bitmap_verify_bitset(&bm,0): %d\n",bitmap_verify_bitset(&bm,8));
    
    //从8开始申请1个bit
    bitmap_alloc_cont_bits(&bm, 8, 4);
    
    //从10开始回收2个bit
    bitmap_recycle_cont_bits(&bm, 10, 2);
    
    int i=0;
    for(i=0;i<8;i++){
        printf("i %d: byte %x\n",i,bm.bits[i]);
    }
	
}
