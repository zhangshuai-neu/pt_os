#include "refer.h"

int main(){

	char str[]="system call：ptsc_print_str\0";
	ptsc_print_str(str);

	int * p=(int *)0x280000;
	*p=0x55aa;
	
	while(1){}
	return 0;
}
