/*
 * 中断头文件
 *
 * 中断相关,类型定义,函数声明
 *
 * author:Shuai Zhang (zhangshuaiisme@gmail.com)
 */

#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "std_type_define.h"

#define PIC_M_CTRL 0x20	       //这里用的可编程中断控制器是8259A,主片的控制端口是0x20
#define PIC_M_DATA 0x21	       //主片的数据端口是0x21
#define PIC_S_CTRL 0xa0	       //从片的控制端口是0xa0
#define PIC_S_DATA 0xa1	       //从片的数据端口是0xa1
#define IDT_DESC_CNT 0x21      //目前总共支持的中断数
#define EFLAGS_IF   0x00000200  // eflags寄存器中的if位为1
#define GET_EFLAGS(EFLAG_VAR)   asm volatile("pushfl; popl %0" : "=g" (EFLAG_VAR))  //flag读取寄存器


/*中断门描述符结构体*/
struct gate_desc {
   uint16_t    func_offset_low_word;
   uint16_t    selector;
   uint8_t     dcount;   //此项为双字计数字段，是门描述符中的第4字节。此项固定值，不用考虑
   uint8_t     attribute;
   uint16_t    func_offset_high_word;
};


/* 定义中断的两种状态:
 * INTR_OFF值为0,表示关中断,
 * INTR_ON值为1,表示开中断 */
enum intr_status {		 // 中断状态
    INTR_OFF,			 // 中断关闭
    INTR_ON		         // 中断打开
};

//w函数指针，中断处理函数
typedef void* intr_handler;

void idt_init(void);	//中断初始化

// 注册中断处理函数
void register_handler(uint8_t vector_no, intr_handler function);

enum intr_status intr_set_status(enum intr_status status);
enum intr_status intr_get_status();
enum intr_status intr_enable();
enum intr_status intr_disable();

#endif
