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

	make -j8

---------------------------------

Source Code
----
**bochs_conf:** It include the bochs's base configure file "bochsrc" and log 
file "bochsrc". When you use Makefile, a new bochsrc file will be created
according to you pc's user name.

**boot:** 

**build:**

**dev:** 

**doc:** 

**it:** 

**kernel:**

**lib:**

**sc:**
	

#### Code Notice

The variable in code will be initialized by 0, and I add the option of 
-fno-zero-initialized-in-bss in gcc C_FLAGS. Therefore the variable will be put
in .data segment.

#### User Notice

You should put "pt_os" dir and "bochs" dir in current user's Desktop, 
otherwise you need to modify "bochsrc" and "Makefile".
























