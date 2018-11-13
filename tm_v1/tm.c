/*
 * 任务管理 实现
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "std_type_defines.h"
#include "system_call.h"
#include "list.h"
#include "bitmap.h"

#include "tm.h"


struct task * main_task; 
struct list_node* task_ready_list;	// 就绪链表
struct list_node* task_list;	    // 所有任务链表


//============ 内核线程部分 ==================
// 所有函数 thread开头


//任务结构 52 byte
struct task{
    uint16_t                task_id;                    //任务id, 从1开始
    char                    task_name[TASK_NAME_LEN];   //任务名
    
    enum task_status        status;                     //状态
    
    uint8_t                 priority;                   //优先级
    uint32_t                elapsed_ticks;              //从开始执行所使用tick数
    
    uint32_t * 			    kstack;                     //task的内核堆栈
    struct bitmap           virt_bitmap;                    //bitmap指针，任务的虚拟地址空间
    
    uint32_t* pgdir;         // 进程自己页目录的虚拟地址
    uint32_t  stack_magic;   // 用这串数字做栈的边界标记,用于检测栈的溢出
    
    struct list_node general_link; // 在一般的队列中的结点
    struct list_node task_link;    // 在线程队列中的结点
}; 

//返回一个初始化好的 thread task结构
struct task* thread_init(char* task_name, uint8_t prio){
    struct task * tp = NULL;
    int i =0;
    for(;i<TASK_MAX_NUM;i++){
        tp = struct task *(TASK_BASE_ADDR+i*SIZE_4K*2)
        if(tp->task_id == 0){
            // 空闲的task区域
            tp->task_id = i+1;  //不等于0
            ptsc_strcpy(tp->task_name,task_name);
            tp->status = TASK_READY;
            tp->priority = prio;
            tp->elapsed_ticks = 0;
            tp->kstack = (uint32_t*)((uint32_t)tp + SIZE_4K);
            
            // thread的bitmap结构为空，process再添加
            ptsc_memset(&virt_bitmap,0,sizeof(struct bitmap);
            
            /*
            tp->virt_bitmap.bits = (uint32_t*)((uint32_t)tp + SIZE_4K);
            tp->virt_bitmap.btmp_bytes_len = SIZE_4K;
            init_bitmap(&(tp->virt_bitmap));
            */
            
            tp->pgdir = NULL;
            tp->stack_magic = 0x19941129;
        }
    }
    return tp;
}

// 将 kernel/main.c 下的main函数作为 主线程
void thread_make_main(struct task * main_task){
    // main线程的特别修改
    main_task->status = TASK_RUNING;
    main_task->
    
}


void thread_environment_init(){
    //初始化 task结构的存储
    struct task * tp = NULL;
    int i =0;
    for(;i<TASK_MAX_NUM;i++){
        tp = struct task *(TASK_BASE_ADDR+i*SIZE_4K*2)
        tp->task_id = (uint16_t)0;
    }
    
    //创建 main_task
    struct task * main_task = thread_init("main",1);
    if(main_task==NULL){
        ptsc_print_str("thread_init() failed\n");
    }
    thread_make_main(main_task);
    
    
    
    
}




//============ 进程部分 =====================
// 所有函数 process开头
