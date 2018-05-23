ProtoType Operating System(PT-OS)
====

This is a simple Operating System, Which implements some useful component.
All componnet of PT-OS is just ProtoType, and that is origin of its name.

When you have some intersting ideas, and you can implement them in PT-OS.
And I think it will reduce the waste of time. 

PTOS target:
----
1)Multi-Core 
		a)single intel core
		b)two same intel cores (It's not ok)
		c)two intel cores with different frequency (It's not ok)
		d)

2)内存管理
		a)简化的slab page

3)




代码注意：
	初始化变量都初始化为0，gcc加入了-fno-zero-initialized-in-bss选项，会将其编入.data段

使用注意：将pt_os和bochs目录全放到当前用户的Desktop目录下

源码目录：
	bochs_conf:
		bochs的配置、日志
	boot:
		启动
	build:
		编译
	dev:
		设备
	doc:
		文档
	it:
		中断
	kernel:
		内核初始化
	lib:
		库函数
	sc:
		系统调用

