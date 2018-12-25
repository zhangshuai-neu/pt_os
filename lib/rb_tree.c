/*
 * 参考：
 *  算法导论
 *  https://www.cnblogs.com/yyxt/p/4983967.html
 *  https://www.cnblogs.com/skywang12345/p/3245399.html
 * 
 */

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
                    //这次旋转的节点更新，在下次循环开头进行
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

// tree的最小节点
struct rb_tree * rb_minimum(struct rb_tree * root){
    while(root && root->left)
        root = root->left;
    return root;
}

// 后继节点，比x大的最小节点y
struct rb_tree * rb_successor(struct rb_tree * x){
    if(x==NULL)
        return NULL;
    //x存在右孩子
    if(x->right!=NULL)
        return rb_minimum(x->right);
    
    // x是父亲的左儿子，则直接结束，因为父亲节点是第一个比他大的节点
    struct rb_tree * y = x->parent;
    while(y && x==y->right){
        // x是父亲的右儿子，x比父亲节点大，判断祖父节点是否会比x大
        // 如果没有祖先节点比x大，则返回NULL(root->parent)
        x=y;
        y=x->parent;
    }
    return y;
} 

// 移植，将树v替换树u
// u_node必须存在
struct rb_tree * rb_transplant(struct rb_tree * root, struct rb_tree * u_node, struct rb_tree * v_node){
    if(u_node->parent == NULL){
        root = v_node;
    } else {
        if(u_node == u_node->parent->left){
            u_node->parent->left = v_node;
        } else {
            u_node->parent->right = v_node;
        }
    }
    if(v_node)
        v_node->parent =  u_node->parent;
    return root;
}

// 删除节点后的修复
                /*
                    // notice: 代码图中[]为x节点，-r表示红色，-b表示黑色,-*表示不确定
                    情形0: x是红色节点
                    (01) 将x节点变成黑色，则全树平衡
                                
                    说明：
                    x节点侧由于删除节点时，删除的y为黑色，所以a侧的黑高比另一侧黑高低
                    直接讲x置位黑色，就能实现黑高的加1，最终达到平衡
                */
                /*  
                    // notice: 代码图中[]为x节点，-r表示红色，-b表示黑色,-*表示不确定
                    情形1: x是黑色节点，x的兄弟节点是红色。(此时x的父节点和x的兄弟节点的子节点都是黑节点)。
                    (01) 将x的兄弟节点设为“黑色”。
                    (02) 将x的父节点设为“红色”。
                    (03) 对x的父节点进行左旋。
                    (04) 左旋后，重新设置x的兄弟节点。

                    说明：
                    因为d为红色，d的子节点全为黑色
                    该种情形一定会转化为2,3,4中的一种
                    只有a，c节点都为黑色，才有可能使用左旋来增加，a节点一侧的黑高
                        ...                    ...
                         |                      |   
                        b-b                    d-b
                        /   \                  /   \ 
                    [a-b]    d-r    ->      b-r    e-b   -> 变成情形2,3,4中的一种
                            /    \         /   \  
                        c-b   e-b     [a-b]  c-b   
                */
                /*
                    // notice: 代码图中[]为x节点，-r表示红色，-b表示黑色,-*表示不确定
                    情形2: x是黑色节点，x的兄弟节点是黑色，x的兄弟节点的两个孩子都是黑色。
                    (01) 将x的兄弟节点设为“红色”。
                    (02) 设置“x的父节点”为“新的x节点”。

                    说明：
                    a侧左子树由于删除节点时，删除的y为黑色，所以a侧的黑高比d侧黑高低
                    通过将d的颜色变成红色，将两侧的黑高一致化
                    但是b节点相对其兄弟节点，黑高还是少了1
                    如果b是红色，将其置为黑色，则b节点相对其兄弟节点黑高一致，全树平衡
                    如果b是黑色，将b当做x继续处理
                        ...                    ...
                         |                       |          
                        b-*                    [b-*]
                        /   \                  /    \       ->     如果b为红色(b相对其兄弟节点，少一个黑色节点)，将其改为黑色，则全树平衡
                    [a-b]    d-b    ->       a-b      d-r          如果b为黑色，变成情形1,2,3,4中的一种
                            /    \                   /    \   
                           c-b   e-b               c-b   e-b  
                */
                /*
                    // notice: 代码图中[]为x节点，-r表示红色，-b表示黑色,-*表示不确定
                    情形3: x是黑色节点，x的兄弟节点是黑色；x的兄弟节点的左孩子是红色，右孩子是黑色的。
                    (01) 将x兄弟节点的左孩子设为“黑色”。
                    (02) 将x兄弟节点设为“红色”。
                    (03) 对x的兄弟节点进行右旋。
                    (04) 右旋后，重新设置x的兄弟节点。

                    说明：
                    在保持d侧黑高不变的情况下，增加d侧的树高，为情况4做准备

                        ...                      ...
                         |                        |          
                         b-*                      b-*
                        /   \                   /   \    
                    [a-b]    d-b    ->       [a-b]  c-b  -> 情况4
                            /    \                   \
                            c-r   e-b                 d-r
                                                        \
                                                        e-b
                        情形3处理的目的是为了增加右子树的黑高
                */
                /*
                    情形4: x是黑色节点，x的兄弟节点是黑色；x的兄弟节点的右孩子是红色的，x的兄弟节点的左孩子任意颜色。
                    (01) 将x父节点颜色 赋值给 x的兄弟节点。
                    (02) 将x父节点设为“黑色”。
                    (03) 将x兄弟节点的右子节设为“黑色”。
                    (04) 对x的父节点进行左旋。
                    (05) 设置“x”为“根节点”。

                    说明：
                    通过对b左旋，增加a节点一侧的黑高（保持e节点一侧黑高不变）

                       ...                    ...
                        |                      |
                        b-*                   [d-*]
                       /   \                 /     \
                    [a-b]    d-b    ->     b-b     e-b
                            /    \        /   \
                           c-r/b e-r    a-b  c-r/b
                            
                        情形4处理的目的是为了增加左子树的黑高(不破坏左子树黑高的基础上)
                */
// 修复删除的影响
// 代码按照图中示例进行编写
struct rb_tree * fix_remove(struct rb_tree * root, struct rb_tree * x_node){
    struct rb_tree * b_node = NULL;
    struct rb_tree * d_node = NULL;

    while(x_node!=root && x_node!=BLACK){
        // 获取b和d节点
        b_node = x_node->parent;
        if(x_node == b_node->left)
            d_node = b_node->right;
        else
            d_node = b_node->left;

        if(x_node == b_node->left){
            //x节点是父亲节点的左
            if(d_node->color==RED){
                //情形1
                //x是黑色节点，x的兄弟节点是红色。(此时x的父节点和x的兄弟节点的子节点都是黑节点)。
                d_node->color = BLACK;
                b_node->color = RED;
                left_roate(b_node);
                if(d_node->parent==NULL)
                    root = d_node;
                
                //每次旋转之后，更新节点位置
                b_node = x_node->parent;
                if(x_node == b_node->left)
                    d_node = b_node->right;
                else
                    d_node = b_node->left;
                
            } 
            if( (d_node->left==NULL || d_node->left->color==BLACK) && (d_node->right==NULL || d_node->right->color==BLACK) ){
                //情形2
                //x是黑色节点，x的兄弟节点是黑色，x的兄弟节点的两个孩子都是黑色。
                if(b_node->color==RED){
                    b_node->color==BLACK;
                } else {
                    x_node = b_node;
                }
            } else {
                if( (d_node->left!=NULL || d_node->left->color==RED) && (d_node->right==NULL || d_node->right->color==BLACK) ){
                    //情形3
                    //x是黑色节点，x的兄弟节点是黑色；x的兄弟节点的左孩子是红色，右孩子是黑色的。
                    d_node->left->color = BLACK;
                    d_node->color = RED;
                    right_roate(d_node);

                    //每次旋转之后，更新节点位置
                    b_node = x_node->parent;
                    if(x_node == b_node->left)
                        d_node = b_node->right;
                    else
                        d_node = b_node->left;
                }
                //情形4
                //x是黑色节点，x的兄弟节点是黑色；x的兄弟节点的右孩子是红色的，x的兄弟节点的左孩子任意颜色。
                d_node->color = b_node->color;
                d_node->color = BLACK;
                d_node->right->right == BLACK;
                left_roate(d_node);
                if(d_node->parent==NULL)
                    root = d_node;
                //这次旋转之后的节点更新，放在循环开头进行
            }
        // left end
        } else {

        } //right end
        
    }   //while end
    
    //情形0
    x_node->color = BLACK;

    return root;
}

// 从rb_tree中删除节点
struct rb_tree * rb_tree_remove(struct rb_tree * root, struct rb_tree * z_node){
    struct rb_tree * x_node = NULL;
    struct rb_tree * y_node = z_node;
    enum rb_color y_origin_color = y_node->color;

    if(z_node->left == NULL){
        // 没有孩子节点，或者只有右孩子
        x_node = z_node->right;
        root = rb_transplant(root, z_node, x_node);
    } else {
         if(z_node->right == NULL){
            // 没有孩子节点，或者只有左孩子
            x_node = z_node->left;
            root = rb_transplant(root, z_node, x_node);
         } else {
            // z有两个孩子节点, y为z的后继节点
            // y已经是z的右二子的最左节点
            y_node = rb_successor(z_node);
            y_origin_color = y_node->color;

            // y一定没有左儿子,y可能有右儿子或者没有孩子 
            x_node = y_node->right;

            if(y_node->parent = z_node){
                // y的父亲节点是z，直接移动y替换z
                // 不需要在z,y的值进行交换，在进行删除
                root = rb_transplant(root, z_node, y_node);
                // 将z的孩子节点连接到y上 (y就是z的右孩子，所以只处理左孩子)
                y_node->left = z_node->left;
                y_node->left->parent = y_node;
            } else {
                // 将y节点放置在叶子节点的位置
                root = rb_transplant(root, y_node, x_node);
                
                // 将z的孩子节点连接到y上
                y_node->right = z_node->right;
                y_node->right->parent = y_node;

                y_node->left = z_node->left;
                y_node->left->parent = y_node;
                
                // 将y子树，替换z子树(z子树只有z自己)
                root = rb_transplant(root, z_node, y_node);
            }
            // 为了减少红黑树规则的违反数量，将y节点的颜色，设置为z节点的颜色
            y_node->color = z_node->color;
         }

    }

    /* 
     * z节点删除之后可能会违反性质2,4,5 违反的例子
     * 违反性质2：删除z且z是根，而y是红色的 => 违反 根节点为黑色
     * 违反性质4：y为黑色，y的父亲为红色, x为红色 => 违反 红色节点的孩子节点为黑色
     * 违反性质5：z为黑色,删除了黑色节点 => 违反 节点到叶子节点的简单路径的黑高不同
     * 
     * 删除造成不平衡的本质在于，y节点所在一侧的黑高减少了1
     * 
     */
    if(y_origin_color == BLACK)
        fix_remove(root, x_node);

    return root;
}

