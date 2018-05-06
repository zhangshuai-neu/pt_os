#include "refer.h"

int main(){

	//1. 初始化显示亮度(字符模式)
	ptsc_init_view();

	char str[]="system call: ptsc_print_str\n";
	ptsc_print_str(str);
	ptsc_print_str(str);

	int * p=(int *)0x280000;
	*p=0x55aa;
	
	while(1){
		
	}
	return 0;
}
