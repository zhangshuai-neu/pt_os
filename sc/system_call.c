/*
 * 系统调用
 *
 * 为了实现一些重复的内核功能，创建一些系统调用，用户程序可以通过中断进行访问。
 * 所有的系统调用以ptsc作为前缀，表示ProtoType_System_Call
 * 
 * 实现的系统调用列表:
 * 1.ptsc_init_view 初始化亮度(字符模式)
 * 2.ptsc_print_str 打印字符串(字符模式)
 * 3.
 * 4.
 *
 * Author:Shuai Zhang
 * Email: zhangshuaiisme@gmail.com
 */

//头文件
#include "stdint.h"
#include "system_call.h"

/*
 * 全局变量(当前空白位置)
 * 不能初始化为0，因为我没有写elf解释程序，所有bss段不会被分配内存
 * 会造成变量不确定的情况(严重错误)
 */
uint16_t view_row=1;				//current row	当前行
uint16_t view_column=1;			//current column 当前列

/*
 * ID: 			1
 * Comment: 	初始化亮度(字符模式)
 *				全为亮白色
 *
 * Name: 		ptsc_init_view
 * Parameter: 	nothing	
 * Return: 		nothing
*/
void ptsc_init_view(){
	char *color_addr=(char*)VIEW_MEM_BASE_ADDR;
	int i=0,j=0;
	
	for(i=0;i<=VIEW_ROW_MAX;i++){
		for(j=0;j<=VIEW_COLUMN_MAX;j++){
			*(color_addr+j*2+1)=0x0f;
		}
		color_addr+=VIEW_ROW_SIZE;
	}
	
	/*
	 * 真正的初始化
	 */
	view_row=2;
	view_column=0;
}

/*
 * ID: 			2
 * Comment: 	打印一个字符串(在字符模式下，范围是80*24)
 * 				字符串以 \0 结尾，\n 换行
 *				c语言的字符串结尾自动添加\0，不需要手动添加
 *				
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
			c_addr++;
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
		for(i=0;i<=VIEW_ROW_MAX;i++){
			for(j=0;j<=VIEW_COLUMN_MAX;j++){
				*(pre_line_addr+j*2)=*(next_line_addr+j*2);
			}
			pre_line_addr+=VIEW_ROW_SIZE;
			next_line_addr+=VIEW_ROW_SIZE;
		}
		//清空底行
		for(j=0;j<=VIEW_COLUMN_MAX;j++){
			*(pre_line_addr+j*2)=0;
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
			return ;
		}
		
		//列满？
		if(view_column > VIEW_COLUMN_MAX){
			view_row++;
			view_column=0;
		}

		//行满?
		if(view_row > VIEW_ROW_MAX){
			up_line();
			view_row=VIEW_ROW_MAX;
			view_column=0;
		}
		
		//写内存（每个显示单元占2个字节）
		view_addr+=VIEW_ROW_SIZE*view_row; 	//行
		view_addr+=view_column * 2;			//列
		*view_addr=c;						//写
		
		view_column+=1;						//光标后移
	}






