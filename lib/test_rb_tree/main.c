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
    printf("%d ",key);
}
//================================================


int main(){
    struct task t[20];
    struct rb_tree * root=NULL;

    int i=0;
    for(i=0;i<10;i++){
        t[i].val = rand()%100;
        rb_tree_init( &(t[i].rb_node) );
        root = rb_tree_insert( root, &(t[i].rb_node) );
        printf("%d ",t[i].val);
    }

    printf("\n rb_traversal_inorder: \n");
    rb_traversal_inorder(root);
    printf("\n rb_traversal_preorder: \n");
    rb_traversal_preorder(root);
    printf("\n rb_traversal_postorder: \n");
    rb_traversal_postorder(root);
/*
    for(i=0;i<5;i++){
        root = rb_tree_remove( root, &(t[i].rb_node) );
    }
    printf("\n rb_traversal_inorder: \n");
    rb_traversal_inorder(root);
    printf("\n rb_traversal_preorder: \n");
    rb_traversal_preorder(root);
    printf("\n rb_traversal_postorder: \n");
    rb_traversal_postorder(root);
*/
    return 0;
}
