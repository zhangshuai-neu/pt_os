#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../rb_tree.h"

//将rb_tree
struct task{
    struct rb_tree rb_node;
    int val;
};

//================================================
// 实现rb_tree要求的两个函数 
//================================================

//key是32位整数
int rb_tree_get_key(struct rb_tree * node){
    // 注意对GET_KEY宏的使用
    return RB_GET_KEY(struct task, node, rb_node, val);
}

void rb_tree_show(struct rb_tree * node){
    int key = rb_tree_get_key(node);
    printf("%d",key);
}
//================================================


int main(){
    

    return 0;
}
