/*
 * kernel初始化
 *
 * 完成内核启动需要的一些必要初始化工作
 *
 * 0.汇编完成的前期工作
 * 1.self define 自定义的函数
 * 2.interrupt 中断处理
 * 3.timer 时钟初始化
 * 
 * mm 内存管理
 * tm 任务管理
 * fs 文件系统
 * 
 * 6.system_call 必要的功能性重用函数
 *
 * author:Shuai Zhang
 * email:zhangshuaiisme@gmail.com
 */

#include "system_call.h"
#include "interrupt.h"
#include "timer.h"

int main(void){
	
	//1. 初始化显示亮度(字符模式)
	ptsc_init_view();
	ptsc_print_str("Init View: OK\n");
	
	//0.汇编完成的前期工作
	ptsc_print_str("Get Memmory Size: OK\n");
	ptsc_print_str("Get into Protection Mode: OK\n");
	ptsc_print_str("Page Management: OK\n");	//完成部页表
	
	
		
	//2. 中断初始化
	idt_init();
	ptsc_print_str("Idt Init: OK\n");
	
	//3. 时钟初始化
	timer_init();
	ptsc_print_str("Timer Init: OK\n");
	

	//4. ##初始化

	while(1){
	
	}
	return 0;
}
