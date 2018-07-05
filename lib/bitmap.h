#ifndef BITMAP_H
#define BITMAP_H

#include "std_type_define.h"
#inlcude ""

struct bitmap {
   uint32_t btmp_bytes_len;
	
/* 在遍历位图时,整体上以字节为单位,细节上是以位为单位,所以此处位图的指针必须是单字节 */
   uint8_t* bits;
};

void bitmap_init(){
	
}

#endif
