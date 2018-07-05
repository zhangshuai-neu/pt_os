/*
 * 内存管理
 * 
 * 管理方式 —— 使用位示图的方式进行管理
 * 
 * 管理地址范围：
 * 	物理地址 4MB-128MB
 *
 * Author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#include "std_type_define.h"
#include "bitmap.h"

// boot/boot_parameter.inc 下有说明
#define MEM_SIZE_RECORD_ADDR	0xa000





