/*
 * 任务管理 头文件
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef TM_H
#define TM_H

#include "list.h"

// 通用函数类型
typedef void thread_func(void*);

/*********** 任务栈 task_stack  ***********
 * 线程自己的栈,用于存储线程中待执行的函数
 * 此结构在线程自己的内核栈中位置不固定,
 * 用在switch_to时保存线程环境。
 * 实际位置取决于实际运行情况。
 ******************************************/
struct task_stack {
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edi;
	uint32_t esi;

	// 线程第一次执行时,eip指向待调用的函数kernel_thread
	// 其它时候,eip是指向 thread_switch_to 的返回地址
	void (*eip) (thread_func* func, void* func_arg);    //esp+20的位置

	// 参数unused_ret只为占位置充数为返回地址
	void (*unused_retaddr);
	thread_func* function;   // 由Kernel_thread所调用的函数名
	void* func_arg;    		 // 由Kernel_thread所调用的函数所需的参数
};

/***********   中断栈intr_stack   ***********
 * 此结构用于中断发生时保护程序(线程或进程)的上下文环境:
 * 进程或线程被外部中断或软中断打断时,会按照此结构压入上下文
 * 寄存器,  intr_exit中的出栈操作是此结构的逆操作
 * 此栈在线程自己的内核栈中位置固定,所在页的最顶端
********************************************/
struct intr_stack {
	// interrupt.S 中的主动压栈内容
    uint32_t vec_no;	 // 宏VECTOR中push %1压入的中断号
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;	 // 虽然pushad把esp也压入,但esp是不断变化的,所以会被popad忽略
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

	// 中断硬件自动压栈
	// err_code会被压入在eip之后
    uint32_t err_code;
    void (*eip) (void);
    uint32_t cs;
    uint32_t eflags;
    void* esp;
    uint32_t ss;
};

//任务状态
enum task_status{
	TASK_READY,		//就绪
	TASK_RUNING,	//执行
    TASK_BLOCKED    //阻塞
	TASK_WAITING,	//等待
    TASK_HANGING,   //悬挂
	TASK_END		//结束
}


#define TASK_MAX_NUM   128          //任务的最大数量
#define TASK_BASE_ADDR 0x300000     //任务结构基地址
#define TASK_NAME_LEN  16           //任务名字长度
#define TASK_DO_TICKS  10　          //任务进行的tick数量(一个单位)

#define OPEN_FILE_MAX_NUM 8         //打开文件的最大数量

//任务结构 50 byte
struct task{
    uint32_t * 			    	  kstack;                     //task的内核堆栈, 栈所在页的地址
    uint8_t                 task_id;                    //任务id, 从1开始
    char                    task_name[TASK_NAME_LEN];   //任务名
    enum task_status        status;                     //状态
    uint8_t                 priority;                   //优先级
    uint32_t                elapsed_ticks;              //从开始执行所使用tick数
    struct bitmap           virt_bitmap;                //任务的虚拟地址空间
    uint32_t* pgdir;         		 // 进程自己页目录的虚拟地址
    struct list_node all_link;   // 在一般的队列中的结点
    struct list_node ready_link; // 在线程队列中的结点
		uint32_t  stack_magic;   		 // 用这串数字做栈的边界标记,用于检测栈的溢出
																 // 判断栈的操作是否覆盖了task结构
};

//======================================================
// 函数声明

// 确定当前任务的task结构所在位置
struct task_struct* thread_get_task_struct();

// 返回一个初始化好的 thread task结构,返回NULL表示失败
// task_id为0表示该结构未被使用
struct task* thread_init(char* task_name, uint8_t prio);

// 撤销一个任务
void thread_destroy(uint8_t task_id);

//　线程环境初始化
void thread_environment_init();


#endif
