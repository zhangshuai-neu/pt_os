#include <stdio.h>
#include "../bitmap.h"
#include "std_type_define.h"

int main(){
	struct bitmap bm;
	char * new_bits = (char *)malloc(128);
	bm.btmp_bytes_len = 128;
	bm.bits = new_bits;
	
	
	
}
