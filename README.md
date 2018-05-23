ProtoType Operating System(PT-OS)
====

This is a simple Operating System, Which implements some useful component.
All componnet of PT-OS is just ProtoType, and that is origin of its name.
When you have some intersting ideas, and you can implement them in PT-OS.
And I think it will reduce the waste of time. 

PTOS target
----

These are the target that I want to do. 

#### Multi-Core 

- single Intel core (It's working)
- two same Intel cores (In the future)
- two Intel cores with different frequency (In the future)
- single RISC-V 32 core (In the future)

There is a problems that the Hardware-Platform is based on Intel, and I want to
transplant it to other platforms in the future. Recently I am building a SOC 
based on RISC-V ISA, so this maybe a new direction. 

#### Memory Management

- virtual and physical Memory Pool (It's doing)
- simple SLAB page management (It's doing)
- simple Fine-grained Memory Management (It's doing)

#### Task Management

- ELF program load (In the future)
- Kernel process and thread (In the future)
- User process and thread (In the future)
- Simple priority scheduling (In the future)
- CFS scheduling (In the future, and it needs a long time)
- EAS scheduling (In the future, and it needs a long time)

#### Interrupt and Timer Management

- Interrupt controler based on 8258A (It's working)
- Timer based on 8253 (It's working)

---------------------------------

Developing Environment
----

#### Platform and ToolChain

- Linuxmint 17.3/18.3
- GNU gcc 	5.4
- GNU ld 	2.26.1
- GNU Make	4.1
- bochs 	2.6.9

#### Compile Bochs

Fisrt step, you should download "bochs-2.6.9.tar.gz" from 
[Bochs Website](https://sourceforge.net/projects/bochs/files/bochs/2.6.9/)

Second step, you should install some package to solve dependency. <\br>
**command:** sudo apt-get install gcc g++ build-essential xorg-xdev xserver-xorg-dev
libgtk2.0-dev bochs-sdl

Third step, you should configure bochs. <\br>
**command:** ./configure --enable-debugger --enable-disasm --enable-iodebug 
--enable-x86-debugger --with-x --with-x11

Final step, you should compile bochs. <\br>
**command:** make -j8

---------------------------------

Source Code Directory
----

	bochs_conf: bochs的配置、日志
	boot: 启动
	build: 编译
	dev: 设备
	doc: 文档
	it: 中断
	kernel: 内核初始化
	lib: 库函数
	sc: 系统调用

#### Code Notice

初始化变量都初始化为0，gcc加入了-fno-zero-initialized-in-bss选项，会将其编入.data段

#### User Notice

将pt_os和bochs目录全放到当前用户的Desktop目录下
























