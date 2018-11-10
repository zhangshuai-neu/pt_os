/*
 * 任务管理 头文件
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef TM_H
#define TM_H

#include ""

//任务|中断-栈，解决任务切换问题
struct task_stack{

};

//函数栈，解决任务中函数切换问题
struct func_stack{
	//备份寄存器，应用程序二进制接口ABI
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edi;
	uint32_t esi;
	//
};

//任务状态
enum task_status{
	TASK_READY,		//就绪
	TASK_RUNING,	//执行
	TASK_WAITING,	//阻塞
	TASK_END		//结束
}

#define TASK_NAME_LEN 32 //任务名长度

//任务结构
struct task{
    uint16_t                task_id;                    //任务id
    char                    task_name[TASK_NAME_LEN];   //任务名
    enum task_status        status;                     //状态
    uint8_t                 priority;                   //优先级
    task_stack* 			t_stack;                    //task的内核堆栈
    uint8_t                 ticks;	  	  	  	  	  	//每次在处理器上执行的时间嘀嗒数
    uint32_t                elapsed_ticks;              //从开始执行所使用tick数

    struct list_node general_tag;                   /* general_tag的作用是用于线程在一般的队列中的结点 */
    struct list_node all_list_tag;                  /* all_list_tag的作用是用于线程队列thread_all_list中的结点 */
    uint32_t* pgdir;                                // 进程自己页表的虚拟地址
    struct virtual_addr     userprog_vaddr;           // 用户进程的虚拟地址
    struct mem_block_desc   u_block_desc[DESC_CNT];   // 用户进程内存块描述符
    int32_t fd_table[MAX_FILES_OPEN_PER_PROC];      // 已打开文件数组
    uint32_t cwd_inode_nr;                          // 进程所在的工作目录的inode编号
    pid_t parent_pid;                               // 父进程pid
    int8_t  exit_status;                            // 进程结束时自己调用exit传入的参数
    uint32_t stack_magic;                           // 用这串数字做栈的边界标记,用于检测栈的溢出
};

struct schedule_entity{

};




#endif
