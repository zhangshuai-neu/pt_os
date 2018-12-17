#include "rb_tree.h"
#include "std_type_define.h"

void rb_tree_init(struct rb_tree * tree){
    tree->color = BLACK;
    tree->parent = NULL;
    tree->left = NULL;
    tree->right = NULL;
}

// 进行左旋， 父节点变成左儿子
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

// 进行右旋， 父节点变成右儿子
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

/*
-r 表示是红色，-b 表示是黑色

违反性质4，node的父节点都为红色
[]的节点是需要处理的节点

情形1： 
    h是要处理的节点，对g,u,f进行变色

             r-b
           /     \ 
         p-r     ...
       /     \ 
     g-b     a-b
   /   \  
 u-r   f-r
      /   \
    [h-r]   ...
        
            | 变色
            V

             g-b
           /     \ 
         f-r     u-b
       /     \     \ 
    [n-r]     b-b   m-r
   /   \  
 u1-b   f1-b
      /   \
     h1-r  ...
    
    变成情形3

情形2：
             g-b
           /     \ 
        f-r      ...
      /   \ 
    b-b     [n-r]
          /    \  
       u1-b    f1-b
              /     \
             h1-r   ...
        
            | 左旋
            V 

                  g-b
                /     \ 
             n2-r     ...
           /    \  
        [f2-r]    f1-b
       /  \     /     \
     b-b u1-b  h1-r   ...
    
        变成情形3

情形3：
    后缀1表示情形1处理过，n节点变成要处理的节点
    对g,f进行右旋

             g-b
           /     \ 
         f-r     u-b
       /     \     \ 
    [n-r]     b-b   m-r
   /   \  
 u1-b   f1-b
      /   \
     h1-r  ...

            | 右旋
            V
    
           f3-b (性质2变色)
         /      \
      n-r       g3-r
    /    \      /    \
 u1-b   f1-b   b-b   u-b
      /   \           \
     h1-r  ...        m-r
     后缀3表示情形3处理过

*/
// 代码按照上图进行编写,node表示要处理的节点
struct rb_tree * fix_insert(struct rb_tree * root, struct rb_tree * node){
    // 违反性质4，node的父节点都为红色
    // 修复性质4，其他节点可能仍然违反性质4
    struct rb_tree * g_node = NULL; //祖父节点
    struct rb_tree * f_node = NULL; //父节点
    struct rb_tree * u_node = NULL; //叔节点
    while( node->parent && node->parent->color == RED ){
        f_node = node->parent;
        if(f_node->parent){
            g_node = f_node->parent;
            // 父节点是祖父节点的左儿子
            if(f_node == g_node->left){    
                u_node = g_node->right;
                // 性质4-情形1: node的叔节点是红色, 修改颜色避免违反性质5
                // node的叔节点不存在，相当于黑色
                if(u_node && u_node->color==RED){
                    f_node->color = BLACK;
                    u_node->color = BLACK;
                    g_node->color = RED;    //之前一定是黑色的，因为f_node是红色的
                    node = g_node;
                    // g_node之下的节点不违反性质，但是g_node可能违反
                    // 图中，g_node及其父节点，再次违反性质4
                } else {
                    if( node == f_node->right){
                        // 性质4-情形2: node的叔节点为黑，且node为右孩子
                        left_roate(f_node);     //f_node和node进行左旋
                        node = f_node;
                    }
                    // 性质4-情形3: node的叔节点为黑，且node为左孩子
                    f_node = node->parent;      //因为可能是从情形2过来，所以需要更新
                    g_node = f_node->parent;     //因为可能是从情形2过来，所以需要更新

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    right_roate(g_node);    //f_node和node进行右旋
                }
            } else {
            // 父节点是祖父节点的右儿子
                u_node = g_node->left;
                // 性质4-情形1: node的叔节点是红色, 修改颜色避免违反性质5
                // node的叔节点不存在，相当于黑色
                if(u_node && u_node->color==RED){
                    f_node->color = BLACK;
                    u_node->color = BLACK;
                    g_node->color = RED;    //之前一定是黑色的，因为f_node是红色的
                    node = g_node;
                    // g_node之下的节点不违反性质，但是g_node可能违反
                    // 图中，g_node及其父节点，再次违反性质4
                } else {
                    if( node == f_node->left){
                        // 性质4-情形2: node的叔节点为黑，且node为左孩子
                        right_roate(f_node);    //f_node和node进行左旋
                        node = f_node;
                    }
                    // 性质4-情形3: node的叔节点为黑，且node为左孩子
                    f_node = node->parent;      //因为可能是从情形2过来，所以需要更新
                    g_node = f_node->parent;    //因为可能是从情形2过来，所以需要更新

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    left_roate(g_node);    //f_node和node进行右旋
                }
            }
        }
    }
    // 性质2-情形0: node为根节点，上面的调整也可能会造成
    if(f_node->parent==NULL)
        //更新root，情形3处理完成后，f_node会成为最接近root的节点
        root = f_node;  
    root->color = BLACK;
    return root;
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


// node节点的后继(比node大的最小节点)
struct rb_tree * successor( struct rb_tree * node ){
    
}

// 移植，将y移动到x的位置
struct rb_tree * transplant(struct rb_tree * root, struct rb_tree * x_node, struct rb_tree * y_node){

}

// 修复删除的影响
struct rb_tree * fix_remove(struct rb_tree * root, struct rb_tree * out_node){

}

// 从rb_tree中删除节点
struct rb_tree * rb_tree_remove(struct rb_tree * root, struct rb_tree * out_node){

    return root;
}

