#include "rb_tree.h"
#include "std_type_define.h"

void rb_tree_init(struct rb_tree * tree){
    tree->color = BLACK;
    tree->parent = NULL;
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
 / \         =>>>    / \
f   g               e   i
   / \             / \
  h   i           f   h

代码按照上图进行编写
*/
void left_roate(struct rb_tree * e_node){
    //e节点没有右儿子，直接结束
    if(e_node->right==NULL)
        return;

    struct rb_tree * p_node = e_node->parent;
    struct rb_tree * g_node = e_node->right;
    struct rb_tree * h_node = g_node->left;

    e_node->right = h_node;
    g_node->left = e_node;
    if(e_node == p_node->left) {
        p_node->left = g_node;
    } else {
        p_node->right = g_node;
    }
}

// 进行右旋
// 例子，将g节点进行右旋,其实时是 g 和 g->left 的位置进行调整
// 注意:e,g,h的位置变化
/*
      p                   p
    /   \               /   \
   e     b            [g]     b
  / \         <<<=    / \
 f   g               e   i
    / \             / \
   h   i           f   h

代码按照上图进行编写
*/
void right_roate(struct rb_tree * g_node){
    //g节点没有左儿子，直接结束
    if(g_node->left==NULL)
        return;

    struct rb_tree * p_node = g_node->parent;
    struct rb_tree * e_node = g_node->left;
    struct rb_tree * h_node = e_node->right;
    
    g_node->left = h_node;
    e_node->right = g_node;
    if(g_node == p_node->left) {
        p_node->left = e_node;
    } else {
        p_node->right = e_node;
    }
}

/*
 * 修复插入违反的性质：
 * 可能违反的：性质2，性质4
 * notice: node的颜色是红色的
 */

struct rb_tree * fix_insert(struct rb_tree * root, struct rb_tree * node){
    //违反性质4，node的父节点都为红色
    // 性质4-情形1：node的叔节点是红色
    
    // 性质4-情形2: node的叔节点为黑，且为右孩子
    
    // 性质4-情形3: node的叔节点为黑，且为左孩子


    // 性质2-情形0: node为根节点，上面的调整也可能会造成
    root->color = BLACK;

    return root;
}

inline bool rb_tree_is_leaf(struct rb_tree * node){
    return !(node->left || node->right);
}

// 从rb_tree中插入
// 插入之后，红黑树的root可能会变化
struct rb_tree * rb_tree_insert(struct rb_tree * root, struct rb_tree * in_node){

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
    root = fix_insert(root, in_node);

    return root;
}


// 修复删除的影响
struct rb_tree * fix_remove(struct rb_tree * root, struct rb_tree * out_node){
    
}


// 从rb_tree中删除节点
struct rb_tree * rb_tree_remove(struct rb_tree * root, struct rb_tree * out_node){

    return root;
}

