/*
 * 任务管理 实现
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 *
 */

#include "std_type_defines.h"
#include "system_call.h"
#include "list.h"
#include "bitmap.h"
#include "tm.h"

struct task * main_task_ptr; 
struct list_node task_ready_list_head;	// 就绪链表
struct list_node task_list_head;	    // 所有任务链表


//============ 内核线程部分 =============
// 所有函数 thread开头

extern void thread_switch_to(struct task* cur, struct task* next);

// 确定当前任务的task结构所在位置
struct task_struct* thread_get_task_struct() {
   uint32_t esp; 
   asm ("mov %%esp, %0" : "=g" (esp));
   
   // 因为任务的栈在 doc/task_manage.md描述中存储单元中
   // 可以根据stack所在页，确定task结构的位置
   return (struct task_struct*)(esp & 0xfffff000);
}

// 返回一个初始化好的 thread task结构,返回NULL表示失败
// task_id为0表示该结构未被使用
struct task* thread_init(char* task_name, uint8_t prio){
    //error 命名过长
    if(ptsc_strlen(task_name)>TASK_NAME_LEN-1){
        ptsc_print_str("task_errer: task_name too long!\n")
        return NULL;
    }
    
    struct task * tp = NULL;    
    int i = 1;  // 从1开始，空出0任务结构的空间 main_task独立存放
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
            
            //进程的处理
            /*
            tp->virt_bitmap.bits = (uint32_t*)((uint32_t)tp + SIZE_4K);
            tp->virt_bitmap.btmp_bytes_len = SIZE_4K;
            init_bitmap(&(tp->virt_bitmap));
            */
            
            tp->pgdir = NULL;
            tp->stack_magic = 0x19941027;
        }
    }
    return tp;
}

// 撤销一个任务
void thread_destroy(uint8_t task_id){
    struct list_node *temp_node;
    // 从 task_ready_list 中删除
    temp_node = &task_ready_list_head;
    
    // 从 task_all_list 中删除
    
    // 复位task结构
    uint8_t i = task_id-1;
    struct task * tp = (struct task *)(SIZE_4K*2*i);
    tp->task_id = 0;
}

// 将 kernel/main.c 下的main函数作为 主线程
void thread_make_main(){
    main_task_ptr = (struct task*)thread_get_task_struct()
    // main线程的特别修改
    main_task_ptr->kstack = (uint32_t *)((uint32_t)main_task_ptr + SIZE_4K);
    main_task_ptr->task_id = 1;
    ptsc_strcpy(main_task_ptr->task_name, "main_task")

    main_task_ptr->status = TASK_RUNING;
    
    main_task_ptr->priority = 1;
    
    // 后续处理
    main_task_ptr->elapsed_ticks = 0
    main_task_ptr->pgdir = NULL; 
    
    // 初始化队列
    list_init(&(main_task_ptr->all_link));
    list_insert(&task_list_head, main_task_ptr);   // 将main_task加载到task_list中
    list_init(&(main_task_ptr->ready_link));
}


void thread_environment_init(){
    // 初始化列表
    list_init(task_ready_list_head);
    list_init(task_list_head);
    
    // 初始化 task 结构的 task_id
    struct task * tp = NULL;
    int i =0;
    for(;i<TASK_MAX_NUM;i++){
        tp = struct task *(TASK_BASE_ADDR+i*SIZE_4K*2)
        tp->task_id = (uint16_t)0;
    }
    
    // 创建 main_task
    thread_make_main(main_task);
    
    // 注册任务切换的中断
    
}

void schedule(){

}



//============ 进程部分 =====================
// 所有函数 process开头






