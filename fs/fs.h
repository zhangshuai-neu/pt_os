/*
 * 文件系统头文件
 * 
 * 给出一些文件系统的相关大的结构定义
 * 
 * 
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */
#ifndef FS_H
#define FS_H

#include "std_type_define.h"


struct inode{
    uint32_t inode_id;    //文件最多有2^32个
    uint32_t start_block; //起始磁盘块
    uint32_t 
    
};

//文件
struct file{
    char file_name[32];   //文件名最长32个字符
    uint32_t modify_day;  //记录哪一天修改过
    uint8_t access_mode;  //访问权限1(r),2(w),4(x)
    /*
     * 先按照单用户系统进行设计，
     * 不设置属主、属组和others的访问权限
     */
};

//目录
struct dirctory{
    
    
}


/*
 * 目录树，用b树来实现
 * 将整个目录树统一存储，在载入文件系统时，统一读出
 *
 */
struct dir_btree{
    
}


#endif