#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../queue.h"

struct queue_unit{
    int val;
    char name[16];
};

static inline void queue_unit_show( struct queue_unit * qn_ptr ) {
    printf("\n %d \n", qn_ptr->val );
    printf("\n %s \n", qn_ptr->name );
}

void queue_show(struct queue * q_ptr) {
    q->base_addr = base_addr;

    int i=0;
    for(i=q->unit_head; i<q->unit_tail; i++){

    }
}


int main(){
    int unit_size = sizeof(struct queue_unit);
    int unit_num = 5;
    void * queue_base_ptr = malloc( unit_size * unit_num );



    // 测试queue的功能
    struct queue q;
    queue_init(&q, queue_base_ptr, unit_size, unit_num );
    
    queue_send_value()

    queue_receive_value()


    free( queue_base_ptr );
    
    return 0;
}