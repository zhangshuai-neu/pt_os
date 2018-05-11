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

int main(){

	//1. 初始化显示亮度(字符模式)
	ptsc_init_view();



	while(1){
	}
	
	return 0;
}
