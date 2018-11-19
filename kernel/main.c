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
 * author:Shuai Zhang (zhangshuaiisme@gmail.com)
 */
#include "system_call.h"
#include "interrupt.h"
#include "timer.h"
#include "mm.h"
#include "disk_interface.h"
#include "tm.h"

void init_all(){
    //0. 初始化显示亮度(字符模式)
	ptsc_init_view();
	ptsc_print_str("Init View: OK\n");

	//1.汇编完成的前期工作
	ptsc_print_str("Get Memmory Size: OK\n");
	ptsc_print_str("Get into Protection Mode: OK\n");
	ptsc_print_str("Page Management: OK\n");

	//2. 中断初始化
	idt_init();
	ptsc_print_str("Idt Init: OK\n");

	//3. 时钟初始化
	timer_init();		//测试ok
	ptsc_print_str("Timer Init: OK\n");

	//4.设置内存
	kernel_mem_init();
	ptsc_print_str("Kernel Mem Init: OK\n");

	//5.disk接口初始化
	ide_init();
	ptsc_print_str("Ide_init: OK\n");

    //6.线程环境初始化
    thread_environment_init();
    ptsc_print_str("Thread_environment_init: OK\n");
}


/* 在线程A中运行的函数 */
void k_thread_a(void* arg) {
    //用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
    char* para = arg;
    while(1) {
        ptsc_print_str(para);
    }
}

/* 在线程B中运行的函数 */
void k_thread_b(void* arg) {
    /* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
    char* para = arg;
    while(1) {
        ptsc_print_str(para);
    }
}


int main(void){
	//进行所有初始化工作
    init_all();

	//创建task结构，并指明线程执行的函数及参数
	struct task * k_thread_A = thread_init();
	thread_specify_func(k_thread_A,k_thread_a,"A_hi\n");
	struct task * k_thread_B = thread_init();
	thread_specify_func(k_thread_B,k_thread_b,"B_hi\n");

	while(1){
        ptsc_print_str("This is main_task!\n");
	}
	return 0;
}
