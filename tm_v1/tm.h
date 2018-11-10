/*
 * 任务管理 头文件
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef TM_H
#define TM_H

//任务结构基地址
#define TASK_BASE_ADDR 0x300000


//打开文件的最大数量
#define OPEN_FILE_MAX_NUM 8


#define TASK_BASE_ADDR 0x300000

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
	// 其它时候,eip是指向switch_to的返回地址 
	void (*eip) (thread_func* func, void* func_arg);
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
    uint32_t vec_no;	 // kernel.S 宏VECTOR中push %1压入的中断号
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

	//以下由cpu从低特权级进入高特权级时压入
    uint32_t err_code;		 // err_code会被压入在eip之后
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
    uint32_t* 				task_stack;                 //task的内核堆栈
    uint8_t                 ticks;	  	  	  	  	  	//每次在处理器上执行的时间嘀嗒数
    uint32_t                elapsed_ticks;              //从开始执行所使用tick数

    struct list_elem general_tag;                   // general_tag的作用是用于线程在一般的队列中的结点
    struct list_elem all_list_tag;                  // all_list_tag的作用是用于线程队列thread_all_list中的结点
    uint32_t* pgdir;                                // 进程自己页表的虚拟地址
    struct virtual_addr userprog_vaddr;             // 用户进程的虚拟地址
    struct mem_block_desc u_block_desc[DESC_CNT];   // 用户进程内存块描述符
    int32_t fd_table[OPEN_FILE_MAX_NUM];      		// 已打开文件数组
    uint32_t cwd_inode_nr;                          // 进程所在的工作目录的inode编号
    pid_t parent_pid;                               // 父进程pid
    int8_t  exit_status;                            // 进程结束时自己调用exit传入的参数
    uint32_t stack_magic;                           // 用这串数字做栈的边界标记,用于检测栈的溢出
};

struct schedule_entity{

};




#endif
