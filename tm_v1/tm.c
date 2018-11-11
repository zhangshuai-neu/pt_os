/*
 * 任务管理 实现
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "tm.h"

struct task main_task; 
struct list_node* task_ready_list;	// 就绪队列
struct list_node* all_task_list;	// 所有任务队列


/*
 * 调度方法
 * 
 * 1) 轮询调度:  所有任务的优先级相同，循环调度
 * 2) 类cfs调度: 
 *      虚拟执行 virt_tick
 *      实际执行 real_tick
 *      优先级从 1～9，每个优先级的实际运行的单位时间为 (优先级*N，N为固定real_tick数) 
 *      virt_tick小的先执行
 * */



