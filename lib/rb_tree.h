#ifndef RB_TREE_H
#define RB_TREE_H

/**
 * 红黑树是平衡二叉树，left小于root，root小于right
 * 
 * 
 * 红黑树的特性:
 * 1）每个节点或者是黑色，或者是红色。
 * 2）根节点是黑色。
 * 3）每个叶子节点（NIL）是黑色。[注意：这里叶子节点，是指为空(NIL或NULL)的叶子节点]
 * 4）如果一个节点是红色的，则它的子节点必须是黑色的。
 * 5）从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。
 * 
 * 插入的新节点，默认设置为红色(修改次数会比较少)
 * 
 * 不涉及内存接口，只管理申请红黑树节点
 * 
 */

/*
 * 参考：
 *  算法导论
 *  https://www.cnblogs.com/yyxt/p/4983967.html
 *  https://www.cnblogs.com/skywang12345/p/3245399.html
 * 
 */

enum rb_color{
    RED, BLACK
};

struct rb_tree{
    int key;
    enum rb_color color;
    struct rb_tree * parent;
    struct rb_tree * left;
    struct rb_tree * right;
};


// 包含rb_tree的结构key, 指明key的名称
/**
 * 用法：
 * struct task{
 *  ...
 *  struct rb_tree rb_tree_root;
 *  int key;
 *  ...
 * };
 * 
 * 1) 例1
 * rb_tree_root_addr = ...;
 * strcut task * t = RB_TREE_ENTRY(struct task, rb_tree_root_addr , rb_tree_root);
 * key = t->key; //获取key的值
 * 
 * 2) 例2
 * 直接获取key的值
 * int key = GET_KEY(struct task, rb_tree_root_addr, rb_tree_root, key);
 * 
*/

// 获取rb_tree所在结构的基地址, ->的优先级 比 类型转换 要高
#define RB_TREE_ENTRY(TYPE, RB_TREE_ADDR ,RB_TREE_MEMBER) \
    ((TYPE*)((unsigned int)RB_TREE_ADDR - (unsigned int)(&(((TYPE*)0)->RB_TREE_MEMBER))))

// 功能：获取rb_tree所在结构的key
// TYPE: rb_tree所在结构
// RB_TREE_ADDR: rb_tree基地址
// RB_TREE_MEMBER: rb_tree成员名
// KEY: KEY表示key的名字
#define RB_GET_KEY(TYPE, RB_TREE_ADDR, RB_TREE_MEMBER, KEY) \
    (RB_TREE_ENTRY(TYPE, RB_TREE_ADDR ,RB_TREE_MEMBER)->KEY)

// 初始化
void rb_tree_init(struct rb_tree * tree);
// 插入新节点
struct rb_tree * rb_tree_insert(struct rb_tree * root, struct rb_tree * in_node);
// 从rb_tree中删除节点
struct rb_tree * rb_tree_remove(struct rb_tree * root, struct rb_tree * z_node);

// 遍历，并使用 rb_tree_show 函数打印一些内容
void rb_traversal_inorder(struct rb_tree * root);
void rb_traversal_preorder(struct rb_tree * root);
void rb_traversal_postorder(struct rb_tree * root);

//=======================================
// 使用者需要实现的函数
// 为了将key从struct rb_tree结构中解放出来
//======================================
//key是32位整数
int rb_tree_get_key(struct rb_tree * node);
void rb_tree_show(struct rb_tree * node);





#endif 