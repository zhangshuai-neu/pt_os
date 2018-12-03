/*
 * 标准类型和宏定义
 *
 * author:Shuai Zhang <zhangshuaiisme@gmail.com>
 */

#ifndef STD_TYPE_DEFINE_H
#define STD_TYPE_DEFINE_H

//size 常用宏
#define SIZE_4M ((uint32_t)1024*1024*4)
#define SIZE_1M ((uint32_t)1024*1024)
#define SIZE_4K ((uint32_t)1024*4)
#define SIZE_1K ((uint32_t)1024)

//空指针
#define NULL ((void*)0)

// 布尔类型
// size: 4 byte
typedef enum boolean {
	FALSE=0,TRUE
} bool;

//整数
typedef signed 	char 			int8_t;
typedef signed 	short int		int16_t;
typedef signed 	int 			int32_t;

typedef unsigned char 			uint8_t;
typedef unsigned short int		uint16_t;
typedef unsigned int 			uint32_t;

#endif
