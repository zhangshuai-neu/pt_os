#include "queue.h"

// queue 初始化
// unit_size和unit_num 不能为0 
bool queue_init(struct queue * q, void* base_addr, int unit_size, int unit_num){
    if(unit_num==0 || unit_size==0){
        return FALSE; 
    }

    q->base_addr = base_addr;
    q->unit_num = unit_num;
    q->unit_size = unit_size;

    q->unit_head = 0;
    q->unit_tail = 0;

    q->queue_send_block = 1;
    q->queue_receive_block = 1;
    
    return TRUE;
}

bool queue_is_empty(struct queue * q){
    if(q->unit_head == q->unit_tail){
        return TRUE;
    }
    return FALSE;
}


bool queue_is_full(struct queue * q){
    if( q->unit_head == (q->unit_tail+1)%q->unit_num ){
        return TRUE;
    }
    return FALSE;
}

// queue_unit的拷贝
static void queue_unit_copy(char* src_addr, char* dest_addr, int size){
    int i;
    for(i=0;i<size;i++){
        *(src_addr+i) = *(dest_addr+i);
    }
}


// 传值 向队列发送数据
bool queue_send_value(struct queue * q, void * unit_ptr){
    // 满
    if(queue_is_full(q)){
        return FALSE;
    }
    // 未满，向tail处写入的unit，tail后移
    char * unit_dest_addr = q->base_addr;
    unit_dest_addr += ( q->unit_tail * q->unit_size);
    
    queue_unit_copy((char*)unit_ptr, unit_dest_addr, q->unit_size);
    q->unit_tail = (q->unit_tail+1) % q->unit_num;

    return TRUE;
}

// 传值 从队列接收数据
bool queue_receive_value(struct queue * q){
    // 队列为空
    if( queue_is_empty(q) ){
        return FALSE;
    }

}

// 传指针 发送数据
void queue_send_value(struct queue * q ){
    

}

// 传指针 接收数据
void queue_receive_value(struct queue * q){


}