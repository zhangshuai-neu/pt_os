/*
 * 系统调用
 *
 * 为了实现一些重复的内核功能，创建一些系统调用，用户程序可以通过中断进行访问。
 * 所有的系统调用以ptsc作为前缀，表示prototype_system_call
 * 
 * 实现的系统调用列表:
 * 1.ptsc_print_str 打印字符串(字符模式)
 * 2.
 * 3.
 * 
 * Author:Shuai Zhang
 * Email: zhangshuaiisme@gmail.com
 */

#include "system_call.h"

/*
 * ID: 			1
 * Comment: 	打印一个字符串，字符串以 \0 结尾
 *				在字符模式下，范围是80*25
 * Name: 		print_str
 * Parameter: 	str_addr	
 * Return: 		nothing
*/
	void
	void print_char(char c);
	

	void ptsc_print_str(char *str){
		char *c_addr=str;
		while(*c_addr != 0){
			print_char(*c_addr);
		}
	}
	//所有内容上移一行
	void up_line(){
		
	}
	//在当前光标处显示一个字符
	void print_char(char c){
		
		//行满?
		if(view_cr >= 25){
			//列满?
			if(view_cc >= 80){
				up_line();	//全满了
			}
		}
	}






