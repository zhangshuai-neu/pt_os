/*
 * 任务管理 头文件
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef TM_H
#define TM_H

//任务|中断-栈，解决任务切换问题
struct task_stack{

};

//函数栈，解决任务中函数切换问题
struct func_stack{

};


//任务状态
enum task_status{
	TASK_READY,		//就绪
	TASK_RUNING,	//执行
	TASK_WAITING,	//阻塞
	TASK_END		//结束
}

//任务结构
struct task{
	uint16_t					task_id;						//任务id
	enum task_status	status;							//状态
	uint8_t 					priority;						//优先级

	uint32_t 					self_kstack_addr;	//task的内核堆栈

};


struct schedule_entity{

};




#endif
