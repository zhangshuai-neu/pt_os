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
 * Comment: 	打印一个字符串
 * 				字符串以 \0 结尾，\n 换行
 *				在字符模式下，范围是80*25
 *
 * Name: 		print_str
 * Parameter: 	str_addr	
 * Return: 		nothing
*/
	void up_line();
	void print_char(char c);	

	void ptsc_print_str(char *str){
		char *c_addr=str;
		while(*c_addr != 0){
			print_char(*c_addr);
		}
	}
	
	//所有内容上移一行,底行内存清空
	void up_line(){
		int i=0; 	//line 遍历
		int j=0;	//column 遍历
		
		//顶行之后可以保存到日志中
		
		//复制
		char *pre_line_addr=(char *)VIEW_MEM_BASE_ADDR;
		char *next_line_addr=(char *)VIEW_MEM_BASE_ADDR+VIEW_ROW_SIZE;
		for(i=1;i<25;i++){
			for(j=0;j<=79*2;j++){
				*(pre_line_addr+j)=*(next_line_addr+j);
			}
			pre_line_addr+=VIEW_ROW_SIZE;
			next_line_addr+=VIEW_ROW_SIZE;
		}
		//清空底行
		for(j=0;j<=79*2;j++){
			*(next_line_addr+j)=0;
		}
	}

	/*
	 * 在当前光标处显示一个字符
	 * 如果字符是\n 则换行
	 * 
	 */
	void print_char(char c){
		char *view_addr=(char *)VIEW_MEM_BASE_ADDR;
		//换行
		if(c == '\n'){
			view_row++;
			view_column=0;
		}
		
		//列满？
		if(view_column >=80){
			view_row++;
			view_column=0;
		}

		//行满?
		if(view_row >= 25){
			up_line();
			view_row=24;
			view_column=0;
		}
		
		//写内存（每个显示单元占2个字节）
		view_addr+=VIEW_ROW_SIZE*view_row; 	//行
		view_addr+=view_column * 2;			//列
		*view_addr=c;						//写内存
		view_column+=2;						//光标后移
	}






