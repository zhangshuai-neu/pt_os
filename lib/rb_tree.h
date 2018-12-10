#ifndef RB_TREE_H
#define RB_TREE_H

/**
 *  红黑树的特性:
 * （1）每个节点或者是黑色，或者是红色。
 * （2）根节点是黑色。
 * （3）每个叶子节点（NIL）是黑色。[注意：这里叶子节点，是指为空(NIL或NULL)的叶子节点]
 * （4）如果一个节点是红色的，则它的子节点必须是黑色的。
 * （5）从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。
 */

enum rb_color{
    RED,BLACK
};

struct rb_tree{
    enum rb_color color;
    struct rb_tree * left;
    struct rb_tree * right;
};

// 初始化
void rb_tree_init(struct rb_tree * tree);

// 插入节点
void rb_tree_insert(struct rb_tree * tree, struct rb_tree * in_node);

// 删除节点
void rb_tree_remove(struct rb_tree * tree, struct rb_tree * out_node);

// 查找节点
void rb_tree_find(struct rb_tree * tree, struct rb_tree * find_node);

#endif