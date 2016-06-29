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
    bl  nand_init               @初始化NAND Flash

    ldr     r0,     =0x30004000     @1.目标地址=0x30004000，这是SDRAM的的16KB地址处，前16KB用来存放MMU的一级页表4K*4 = 16K，一个页表项对应1M内存
    mov     r1,     #4096           @2.  源地址   = 4096，连接的时候，main.c中的代码都存在NAND Flash地址4096开始处
    mov     r2,     #2048           @3.  复制长度= 2048(bytes)，对于本实验的main.c，这是足够了
    bl      nand_read               @调用C函数nand_read  将NANDFALSH的4096地址后的2K内容拷贝到SDRAM的0x30004000处

    bl  create_page_table               @ 设置页表 需要将开启MMU后需要用到的物理地址全部都映射
    bl  mmu_init                        @ 启动MMU 开启前使用物理地址 开启后使用地址全部是虚拟地址

    ldr sp, =0xB4000000                 @ 重设栈指针，指向SDRAM顶端0x34000000(使用虚拟地址) 64M内存
    ldr lr, =halt_loop
    ldr pc, =main                 @ 跳到SDRAM中继续执行第二部分代码  main.o在mmu.lds里面是连接到B0004000去的: 
				  @所以这里使用ldr pc,=main相当于pc = b0004000 + main函数的相对偏移 
				  @这里是硬件复位启动时将头4K NANDFLASH的内容拷贝到SRAM 然后将其第二部分代码拷贝到SDRAM，第一部分因为不需要载执行
				  @所以无需拷贝但是,所以mmu.lds时将第一部分连接到0地址，第二部分代码连接到B0004000，SDRAM中页表16KB空间后面
				  @程序连接到哪里不一定要下载到哪里，可以在启动的时候由启动文件复制到连接的位置。
halt_loop:
    b   halt_loop

