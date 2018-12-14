#include "rb_tree.h"
#include "std_type_define.h"

void rb_tree_init(struct rb_tree * tree){
    tree->color = BLACK;
    tree->left = NULL;
    tree->right = NULL;
}

// 进行左旋
// 例子，将e节点进行左旋,其实时 e 和 e->right 的位置进行调整
// 注意:e,g,h的位置变化
/*
      p                  p
    /   \              /   \
 [e]     b            g     b
 / \           =>    / \
f   g               e   i
   / \             / \
  h   i           f   h
*/
void left_roate(struct rb_tree * node){
    struct rb_tree * p = node->parent;
    if(node == p->left) {
        
    } else {

    }
}

// 进行右旋
void right_roate(struct rb_tree * node){
    
}

// 修复插入的影响
void fix_insert(struct rb_tree * root, struct rb_tree * node){
    // 情形1: node为根节点
    if (node->parent == NULL){
        node->color = BLACK;
    }
    
    // 情形2:
    
    // 情形3:

}

// 修复删除的影响
void fix_remove(){
    
}

inline bool rb_tree_is_leaf(struct rb_tree * node){
    return !(node->left || node->right);
}

// 从rb_tree中插入
void rb_tree_insert(struct rb_tree * root, struct rb_tree * in_node){

    struct rb_tree * temp_node = root;
    struct rb_tree * temp_node_parent = NULL;

    if(root==NULL){
        // 空树
        root = in_node;
    } else {
        // 遍历一下, 找到in_node的父亲节点，并插入
        while (temp_node){
            temp_node_parent = temp_node;
            if(temp_node->key < in_node->key){
                temp_node = temp_node->left; 
            } else {
                temp_node = temp_node->right;
            }
        }
        if(temp_node_parent->key < in_node->key){
            temp_node_parent->right = in_node; 
        } else {
            temp_node_parent->left = in_node; 
        }
    }

    in_node->color = RED;
    in_node->parent = temp_node_parent;
    in_node->left = NULL;
    in_node->right = NULL;
    
    //维护平衡状态
    fix_insert(root, in_node);
}

// 从rb_tree中删除节点
void rb_tree_remove(struct rb_tree * tree, struct rb_tree * out_node){

}

