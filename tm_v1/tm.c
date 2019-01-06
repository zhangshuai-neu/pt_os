/*
 * 任务管理 实现
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 *
 */

#include "tm.h"

struct task * main_task_ptr;
struct list_node task_ready_list_head;	// 就绪链表
struct list_node task_list_head;	    // 所有任务链表


//============ 内核线程部分 =============
// 所有函数 thread开头
extern void thread_switch_to(struct task* cur, struct task* next);

// kernel_thread,第一次执行时，先进入该函数，然后调用想要执行的function
void kernel_thread(thread_func function, void *func_arg){
    // 进入调度器时，中断被关闭
    // 此时在启动，避免其他任务无法切换
    intr_enable();
    function(func_arg);
}

// 确定当前任务的task结构所在位置
struct task* thread_get_task_struct() {
   uint32_t esp;
   asm ("mov %%esp, %0" : "=g" (esp));

   // 因为任务的栈在 doc/task_manage.md描述中存储单元中
   // 可以根据stack所在页，确定task结构的位置
   return (struct task *)(esp & 0xfffff000);
}

// 返回一个初始化好的 thread task结构,返回NULL表示失败
// task_id为0表示该结构未被使用
struct task* thread_init(char* task_name, uint8_t prio){
    //error 命名过长
    if(ptsc_strlen(task_name)>TASK_NAME_LEN-1){
        ptsc_print_str("task_errer: task_name too long!\n");
        return NULL;
    }

    struct task * tp = NULL;
    int i = 1;  // 从1开始，空出0任务结构的空间 main_task独立存放
    for(;i<TASK_MAX_NUM;i++){
        tp = (struct task *)(TASK_BASE_ADDR+i*SIZE_4K*2);
        if(tp->task_id == 0){
            // 空闲的task区域
            tp->task_id = i+1;  //不等于0
            ptsc_strcpy(tp->task_name,task_name);
            tp->status = TASK_READY;
            tp->priority = prio;
            tp->weight = (uint32_t)prio*TASK_DO_TICKS;
            tp->elapsed_ticks = 0;
            tp->kstack = (uint8_t *)((uint32_t)tp + SIZE_4K);
            //ptsc_memset(&virt_bitmap,0,sizeof(struct bitmap);
            tp->pgdir = NULL;
            list_insert(&task_list_head,&tp->all_link);
            list_insert(&task_ready_list_head,&tp->ready_link);
            tp->stack_magic = 0x19941027;
            break;
        }
    }
    return tp;
}

// 确定线程执行的函数以及参数
void thread_specify_func(struct task * thread_ptr,thread_func run_func, void* func_arg){
    //预留中断栈空间
    (thread_ptr->kstack) -= sizeof(struct intr_stack);

    //预留任务栈空间
    (thread_ptr->kstack) -= sizeof(struct task_stack);
    struct task_stack * t_stack = (struct task_stack *)(thread_ptr->kstack);

    t_stack->eip = kernel_thread;
    t_stack->unused_retaddr  = NULL;
    t_stack->function = run_func;
    t_stack->func_arg = func_arg;
}

// 撤销一个任务
void thread_destroy(uint8_t task_id){
    struct task * tp = (struct task*)(TASK_BASE_ADDR+(task_id-1)*SIZE_4K*2);
    // 从 list 中删除
    list_remove(&(tp->ready_link));
    list_remove(&(tp->all_link));

    // 复位task结构
    tp->task_id = 0;
}

// 将 kernel/main.c 下的main函数作为 主线程
void thread_make_main(){
    main_task_ptr = (struct task*)thread_get_task_struct();
    // main线程的修改
    main_task_ptr->kstack = (uint8_t *)((uint32_t)main_task_ptr + SIZE_4K);
    main_task_ptr->task_id = 1;
    ptsc_strcpy(main_task_ptr->task_name, "main_task");

    main_task_ptr->status = TASK_RUNING;

    main_task_ptr->priority = 10;

    // 后续处理
    main_task_ptr->weight = main_task_ptr->priority * TASK_DO_TICKS;
    main_task_ptr->elapsed_ticks = 0;
    main_task_ptr->pgdir = NULL;

    // 初始化队列
    list_init(&(main_task_ptr->all_link));
    list_insert(&task_list_head, &(main_task_ptr->all_link));
    list_init(&(main_task_ptr->ready_link));

    main_task_ptr->stack_magic = 0x19941027;
}


void thread_environment_init(){
    // 初始化列表
    list_init(&task_ready_list_head);
    list_init(&task_list_head);

    // 初始化 task 结构的 task_id
    struct task * tp = NULL;
    int i =0;
        for(;i<TASK_MAX_NUM;i++){
        tp = (struct task *)(TASK_BASE_ADDR+i*SIZE_4K*2);
        tp->task_id = 0;
    }

    // 创建 main_task
    thread_make_main();
}

void schedule(){
    // 是否关中断
    if(intr_get_status() != INTR_OFF){
        return;
    }

    // 当前任务的时间片是否运行完
    // 运行完，寻找下一个任务，并切换
    struct task * cur_task = (struct task *)thread_get_task_struct();
    struct task * next_task = NULL;
    struct list_node * ready_node =  &task_ready_list_head;

    //如果当前任务正在执行，并且weight已经执行完，则切换到下一个任务
    if(cur_task->status==TASK_RUNING && cur_task->weight<=0){
        cur_task->weight=cur_task->priority*TASK_DO_TICKS;
        if(ready_node->next == ready_node){
            return ;    //ready 队列为空
        }
        ready_node = ready_node->next;
        list_remove(ready_node);
        //获取next task结构
        next_task = list_entry(ready_node,struct task,ready_link);
        next_task->status=TASK_RUNING;
        cur_task->status = TASK_READY;
        list_insert(&task_ready_list_head,&cur_task->ready_link);

        //任务切换
        thread_switch_to(cur_task,next_task);
    }
}

// 当前运行的task,移入block列表中
void thread_block(struct list_node * block_list){
    // 是否关中断
    if(intr_get_status() != INTR_OFF){
        return;
    }

    // 如果当前任务主动被阻塞
    struct task * cur_task = (struct task *)thread_get_task_struct();
    struct task * next_task = NULL;
    struct list_node * ready_node =  &task_ready_list_head;

    if(ready_node->next == ready_node){
        return ;    //ready 队列为空
    }
    ready_node = ready_node->next;
    list_remove(ready_node);
    //获取next task结构
    next_task = list_entry(ready_node,struct task,ready_link);
    next_task->status=TASK_RUNING;
    cur_task->status = TASK_BLOCKED;
    list_insert(&task_ready_list_head, &cur_task->ready_link);
    //任务切换
    thread_switch_to(cur_task,next_task);

    list_insert(block_list,&(cur_task->block_link));
}

//============ 进程部分 =====================
// 所有函数 process开头
