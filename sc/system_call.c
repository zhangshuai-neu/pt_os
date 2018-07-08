/*
 * 系统调用
 *
 * 为了实现一些重复的内核功能，创建一些系统调用，用户程序可以通过中断进行访问。
 * 所有的系统调用以ptsc作为前缀，表示ProtoType_System_Call
 * 
 * 实现的系统调用列表:
 * 
 * 打印操作
 * 1.ptsc_init_view 初始化亮度(字符模式)
 * 2.ptsc_print_str 打印字符串(字符模式)
 * 3 ptsc_print_num16 打印16进制整数
 * 
 * 内存操作
 * 4.ptsc_memcpy 内存复制
 * 5.ptsc_memset 内存置位
 * 
 * 字符串操作
 * 6.ptsc_strlen
 * 7.ptsc_strcmp
 * 8.ptsc_strcpy
 * 
 *
 * Author:Shuai Zhang
 * Email: zhangshuaiisme@gmail.com
 */

//头文件
#include "system_call.h"

/*
 * 全局变量(当前空白位置)
 * 不能初始化为0，因为我没有写elf解释程序，所有bss段不会被分配内存
 * 会造成变量不确定的情况(严重错误)
 */
uint16_t view_row=0;			//current row	当前行
uint16_t view_column=0;			//current column 当前列

/*
 * ID: 			1
 * Comment: 	初始化亮度(字符模式)
 *				全为亮白色
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
	 * 对当前位置，真正的初始化
	 */
	view_row=2;
	view_column=0;
}

/*
 * ID: 			2
 * Comment: 	打印一个字符串(在字符模式下，范围是80*24)
 * 				字符串以 \0 结尾，\n 换行
 *				c语言的字符串结尾自动添加\0，不需要手动添加
*/

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

// 在当前光标处显示一个字符，如果字符是\n 则换行
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

//打印字符串
void ptsc_print_str(char *str){
	char *c_addr=str;
	while(*c_addr != 0){
		print_char(*c_addr);
		c_addr++;
	}
}
	
/*
 * ID: 			3
 * Comment: 	用来获取数字对应字符，空间换时间
 * 				16进制数转字符
*/
char num16_to_char_array[16]={'0','1','2','3','4','5','6','7','8','9','a' \
						,'b','c','d','e','f'};

void ptsc_print_num16(uint32_t num){
	char *out_str="00000000";
	uint32_t i=8;
	uint32_t num_low4bit=0;
	for(;i>0;i--){
		num_low4bit=num & 0x0000000f;
		out_str[i-1]=num16_to_char_array[num_low4bit];
		
		num=num>>4;
	}
	ptsc_print_str(out_str);
}

/*
 * ID: 			4
 * Comment: 	内存拷贝
 * 				以字节为单位
 */
void ptsc_memcpy(void* src_addr,void* dest_addr,uint32_t size){
	if(src_addr != NULL && dest_addr != NULL  && size > 0){
		while(size>0){
			*(uint8_t*)src_addr = *(uint8_t*)dest_addr;
			size--;
		}
	}
	//error处理
}


/*
 * ID: 			5
 * Comment: 	内存设置
 * 				字节为单位
 */
void ptsc_memset(void* dest_addr,uint8_t val,uint32_t size){
	if(dest_addr != NULL  && size > 0){
		while(size>0){
			*(uint8_t*)dest_addr = val;
			size--;
		}
	}
	//error处理
}


/*
 * ID: 			6
 * Comment: 	字符串长度
 */
uint32_t ptsc_strlen(char * str_addr){
	uint32_t len=0;
	if(str_addr!=NULL){
		while(*str_addr != '\0'){
			len++;
			str_addr++;
		}
	}
	//error处理
}

/*
 * ID: 			7
 * Comment: 	字符串比较
 * 
 * str_addr_a > str_addr_b： 1
 * str_addr_a = str_addr_b： 0
 * str_addr_a < str_addr_b： -1
 * 
 */
int8_t ptsc_strcmp(char * str_addr_a,char * str_addr_b){
	int8_t result=0;
	if(str_addr_a!=NULL &&  str_addr_b!=NULL){
		while(*str_addr_a!='\0' && *str_addr_b!= '\0'){
			if(*str_addr_a == *str_addr_b){
				str_addr_a++;
				str_addr_b++;
			} else if(*str_addr_a > *str_addr_b){
				result=1;
				return result;
			} else {
				result=-1;
				return result;
			}
		}
		if(*str_addr_a=='\0' && *str_addr_b=='\0'){
			result=0;
		}else if(*str_addr_b=='\0'){
			result=1;
		} else{
			result=-1;
		}
		
		return result;
	}
	//error处理
}

/*
 * ID: 			8
 * Comment: 	字符串复制
 * 会复制 \0
 */
void ptsc_strcpy(char * src_addr,char * dest_addr){
	if(dest_addr!=NULL && src_addr!=NULL){
		while(src_addr!='\0'){
			*dest_addr=*src_addr;
			dest_addr++;
			src_addr++;
		}
		*dest_addr='\0';
	}
	//error处理
}
