#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../queue.h"

struct queue_unit{
    char name[16];
    int val;
};

static inline void queue_unit_show( struct queue_unit * qn_ptr ) {
    if(qn_ptr == NULL)
        return;
    printf("%s:", qn_ptr->name );
    printf("%x\n", qn_ptr->val );
}

void queue_show(struct queue * q_ptr) {
    if( queue_is_empty(q_ptr) )
        return;

    printf("head:%d\n",q_ptr->unit_head);
    printf("tail:%d\n",q_ptr->unit_tail);
    

    struct queue_unit * base_addr = (struct queue_unit *)q_ptr->base_addr;
    int num = q_ptr->unit_num;
    int i;
    for(i=0;i<num;i++){
        queue_unit_show(base_addr+i);
    }
}


int main(){
    int unit_size = sizeof(struct queue_unit);
    int unit_num = 5;
    void * queue_base_ptr = malloc( unit_size * unit_num );

    

    // 测试queue的功能
    struct queue q;
    queue_init(&q, queue_base_ptr, unit_size, unit_num );
    
    struct queue_unit send_qn;
    struct queue_unit * receive_qn_ptr = (struct queue_unit *)malloc(sizeof(struct queue_unit));
    strcpy(send_qn.name,"ZhangShuai");
    send_qn.val = 0x55aa;
    
    // 发送信息
    queue_send_value(&q,&send_qn);
    queue_send_value(&q,&send_qn);
    queue_send_value(&q,&send_qn);
    queue_send_value(&q,&send_qn);
    printf("send info \n");
    queue_show(&q);

    // 接收信息
    printf("\nreceive info: \n");
    queue_receive_value(&q,receive_qn_ptr);
    queue_show(&q);

    free( queue_base_ptr );
    
    return 0;
}