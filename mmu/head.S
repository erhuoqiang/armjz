@*************************************************************************
@ File：head.S
@ 功能：设置SDRAM，将第二部分代码复制到SDRAM，设置页表，启动MMU，
@       然后跳到SDRAM继续执行
@*************************************************************************       

.text
.global _start
_start:
    ldr sp, =4096                       @ 设置栈指针，以下都是C函数，调用前需要设好栈 SRAM是4K 所以这里是4096
    bl  disable_watch_dog               @ 关闭WATCHDOG，否则CPU会不断重启
    bl  memsetup                        @ 设置存储控制器以使用SDRAM
    bl  copy_2th_to_sdram               @ 将第二部分代码复制到SDRAM
    bl  create_page_table               @ 设置页表
    bl  mmu_init                        @ 启动MMU
    ldr sp, =0xB4000000                 @ 重设栈指针，指向SDRAM顶端(使用虚拟地址)
    ldr pc, =main                 @ 跳到SDRAM中继续执行第二部分代码  leds.o在mmu.lds里面是连接到B0004000去的: 
				  @所以这里使用ldr pc,=main相当于pc = b0004000 + main函数的相对偏移 
				  @这里是硬件复位启动时将头4K NANDFLASH的内容拷贝到SRAM 然后将其第二部分代码拷贝到SDRAM，第一部分因为不需要载执行
				  @所以无需拷贝但是,所以mmu.lds时将第一部分连接到0地址，第二部分代码连接到B0004000，SDRAM中页表16KB空间后面
				  @程序连接到哪里不一定要下载到哪里，可以在启动的时候由启动文件复制到连接的位置。
halt_loop:
    b   halt_loop

