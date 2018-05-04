/*
 * 系统调用
 *
 * 实现系统调用需要的头文件，定义了需要的宏和全局变量，尽量避免使用汇编
 * 
 * 实现的系统调用列表:
 * 1.ptsc_print_str 打印字符串(字符模式)
 * 2.
 * 3.
 * 
 * Author:Shuai Zhang
 * Email: zhangshuaiisme@gmail.com
 */

//1 ptsc_print_str 
	//宏定义
	#define VIEW_MEM_BASE_ADDR	0xc00b8000 	//显示内存基地址
	#define VIEW_ROW_SIZE		(80*2)		//80个字符一行,一个单位需要2 byte
											//ascii 1 byte,color 1 byte
											
	#define VIEW_COLUMN			25			//25行
	
	//全局变量
	unsigned int view_cr=0;		//current row	当前行
	unsigned int view_cc=0；		//current column 当前列
	
//2

