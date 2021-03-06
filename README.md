ProtoType Operating System(PT-OS)
====

This is a simple Operating System, Which implements some useful component.
All componnet of PT-OS is just ProtoType, and that is origin of its name.
When you have some intersting ideas, and you can implement them in PT-OS.
And I think it will reduce the waste of time.

I gave detailed comments in code and design ideas in /doc/ dir. And all of them
is wrote in **chinese**.

PTOS target
----

These are the target that I want to do.

#### Multi-Core

- single Intel core (working)
- two same Intel cores (In the future)
- two Intel cores with different frequency (In the future)
- single Hbird e200 32 core (In the future)

There is a problems that the Hardware-Platform is based on Intel, and I want to
transplant it to other platforms in the future. Recently I am building a SOC
based on RISC-V ISA, so this maybe a new direction.

#### Memory Management

- physical page management (working)
	
	linux(Buddy system), book(bitmap ok) 

- Fine-grained memory management (working)

	FreeRTOS(heap testing), Linux(Slab)

The detailed information about memory  will be showed

#### Task Management

- ELF program load (tesing)
- Kernel thread (ok)
- Simple priority scheduling (ok)
- User process and thread (In the future)
- CFS scheduling (In the future, and it needs a long time)
- RR sechduling ((In the future, and it needs some time))
- EAS scheduling (In the future, and it needs a long time)

#### Interrupt and Timer Management

- Interrupt controler based on 8258A (ok)
- Timer based on 8253 (ok)

#### Debug and Error Handling

- Debug port
- Error Handling module

---------------------------------

Developing Environment
----

#### Platform and ToolChain

- Linuxmint 17.3/18.3
- GNU gcc 	5.4
- GNU ld 	2.26.1
- GNU Make	4.1
- bochs 	2.6.9

If I build the RISC-V SOC successfully, and the "vivado" will be added in tools.

#### Compile Bochs

Fisrt step, you should download "bochs-2.6.9.tar.gz" from
[Bochs Website](https://sourceforge.net/projects/bochs/files/bochs/2.6.9/)

Second step, you should install some package to solve dependency.

	sudo apt-get install gcc g++ build-essential xorg-xdev
	xserver-xorg-dev libgtk2.0-dev bochs-sdl

Third step, you should configure bochs in bochs_dir.

	./configure --enable-debugger --enable-disasm --enable-iodebug
	--enable-x86-debugger --with-x --with-x11

Final step, you should compile bochs in bochs_dir.

	make -j4

---------------------------------

Source Code
----
**bochs_conf:** It includes the bochs's base configure file "bochsrc" and log
file "bochsrc". When you use Makefile, a new bochsrc file will be created
according to you pc's user name.

**boot:** It includes some boot code, such as the MBR and loader program.

**build:** The obj file and elf file will be built here.

**dev:** The device's hardware interface. (timer, disk_interface)

**doc:** Some design document which useful for understanding these code.

**it:** It includes some interrupt controler and some handling program.

**kernel:** It includes the code which are used to start all init and configure.

**lib:** There are some general program and struct, which is helpful for our
development. (list, bitmap, heap, lock, elf, std_type_define)

**mm_v1:** This the simplest memory management system, which uses a bitmap to
manage the physical/virtual memory's usage.(Low Level)

**mm_v2:**	This the more complex memory management system, which uses a Buddy
system to manage the physical memory's usage .(Middle Level)

**sc:** It has some system call. However, these function can only be used by
kernel, because our OS is not completed. In the future , these sys_call will be
occured by system_call handle function and interrupt.


#### Code Notice

The variable in code will be initialized by 0, and I add the option of
**-fno-zero-initialized-in-bss** in gcc C_FLAGS. Therefore the variable 
will be put in .data segment.

#### User Notice

You should put "pt_os" dir and "bochs" dir in current user's Desktop,
otherwise you need to modify "bochsrc" and "Makefile".


#### Book Reference

[1] 30天自己操作系统 </br>
[2] 操作系统真相还原 </br>
[3] linux内核完全注释 </br>
[4] 深入理解linux内核 </br>
[5] 深入理解linux内核架构 </br>
[6] 奔跑吧Linux内核 </br>
[7] 一个64位操作系统的设计与实现 </br>
[8] FreeRTOS源码详解与应用开发——基于STM32 </br>
