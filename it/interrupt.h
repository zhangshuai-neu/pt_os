/*
 * 中断头文件
 *
 * 中断相关h,类型定义,函数声明
 *
 * author:Shuai Zhang
 * email:zhangshuaiisme@gmail.com
 */
 
#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "stdint.h"

#define PIC_M_CTRL 0x20	       // 这里用的可编程中断控制器是8259A,主片的控制端口是0x20
#define PIC_M_DATA 0x21	       // 主片的数据端口是0x21
#define PIC_S_CTRL 0xa0	       // 从片的控制端口是0xa0
#define PIC_S_DATA 0xa1	       // 从片的数据端口是0xa1

#define IDT_DESC_CNT 0x21      // 目前总共支持的中断数

/*中断门描述符结构体*/
struct gate_desc {
   uint16_t    func_offset_low_word;
   uint16_t    selector;
   uint8_t     dcount;   //此项为双字计数字段，是门描述符中的第4字节。此项固定值，不用考虑
   uint8_t     attribute;
   uint16_t    func_offset_high_word;
};
//中断处理函数，函数指针
typedef void* intr_handler;		

void idt_init(void);			//中断初始化

#endif
 
