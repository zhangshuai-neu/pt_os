/*
 * 定时器(计数器)
 *
 * 8253定时器(计数器)的初始化
 *
 * author:Shuai Zhang
 * email:zhangshuaiisme@gmail.com
 */

#include "timer.h"
#include "io.h"
#include "system_call.h"
#include "tm.h"

#define IRQ0_FREQUENCY	   100      //时钟中断频率,1秒100次
#define INPUT_FREQUENCY	   1193180
#define COUNTER0_VALUE	   INPUT_FREQUENCY / IRQ0_FREQUENCY

#define CONTRER0_PORT	   0x40	//计数器0的端口号
#define COUNTER0_NO        0	//选择计数器0
#define COUNTER_MODE	   2	//工作方式2，比率发生器
#define READ_WRITE_LATCH   3	//读写方式(先度低字节，后读高字节)
#define PIT_CONTROL_PORT   0x43	//8253控制端口

//开机以来的时钟数
uint32_t ticks;

/*
 * 设置频率:
 * 把操作的计数器counter_no、读写锁属性rwl、计数器模式counter_mode
 * 写入模式控制寄存器并赋予初始值counter_value
 */
static void frequency_set(uint8_t counter_port, \
                            uint8_t counter_no, \
                            uint8_t rwl, \
                            uint8_t counter_mode, \
                            uint16_t counter_value) {

    /* 往控制字寄存器端口0x43中写入控制字 */
   outb(PIT_CONTROL_PORT, (uint8_t)(counter_no << 6 | rwl << 4 | counter_mode << 1));
    /* 先写入counter_value的低8位 */
   outb(counter_port, (uint8_t)counter_value);
    /* 再写入counter_value的高8位 */
   outb(counter_port, (uint8_t)counter_value >> 8);
}

//时钟的中断处理函数
static void intr_timer_handler(void) {
    struct task* cur_thread = (struct task*)thread_get_task_struct();
    // 检查栈是否溢出
    if (cur_thread->stack_magic != 0x19941027){
        ptsc_print_str("intr_timer_handler: stack overflow\n");
        while(1);
    }
    cur_thread->weight--;
    cur_thread->elapsed_ticks++; // 记录此线程占用的cpu时间嘀
    ticks++;	                 // 从内核第一次处理时间中断后开始至今的滴哒数
                                 //内核态和用户态总共的嘀哒数
    //进行调度
    schedule();
}

//初始化PIT8253:
void timer_init() {
   // 设置8253的定时周期,也就是发中断的周期
   frequency_set(CONTRER0_PORT, COUNTER0_NO, READ_WRITE_LATCH, COUNTER_MODE,  COUNTER0_VALUE);

   // 设置时钟中断的处理函数
   register_handler(0x20,intr_timer_handler);
}
