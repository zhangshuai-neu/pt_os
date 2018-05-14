/*
 * kernel初始化
 *
 * 完成内核启动需要的一些必要初始化工作
 *
 * 1.self define 自定义的函数
 * 2.mm 内存管理
 * 3.tm 任务管理
 * 4.fs 文件系统
 * 5.interrupt 中断处理
 * 6.system_call 必要的功能性重用函数
 *
 * author:Shuai Zhang
 * email:zhangshuaiisme@gmail.com
 */

#include "system_call.h"
#include "interrupt.h"

int main(void){
	//1. 初始化显示亮度(字符模式)
	ptsc_init_view();

	//完成的内容
	ptsc_print_str("Get Memmory Size: OK\n");
	ptsc_print_str("Get into Protection Mode: OK\n");
	ptsc_print_str("Page Management: OK\n");
	ptsc_print_num16(0x00001234);
	
	
	//5. 中断处理
	//idt_init();



	while(1){
	}
	
	return 0;
}
