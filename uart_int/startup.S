@*************************************************************************
@ File：head.S
@ 功能：设置SDRAM，将nandflash第二部分代码复制到SDRAM，设置页表，启动MMU，
@       然后跳到SDRAM继续执行
@*************************************************************************       
.equ DISABLE_IRQ,0x80
.equ DISABLE_FIQ,0x40
.equ SYS_MOD,0x1f
.equ IRQ_MOD,0x12
.equ FIQ_MOD,0x11
.equ SVC_MOD,0x13
.equ ABT_MOD,0x17
.equ UND_MOD,0x1b
.equ USR_MOD,0x10
.equ MOD_MASK,0x1f

@为每个模式分配512KB大小的栈空间
.equ MEM_SIZE,  0x04000000
.equ TEXT_BASE, 0x30000000
.equ _SVC_STACK, (TEXT_BASE + MEM_SIZE - 4)
.equ _IRQ_STACK, ( _SVC_STACK - 0x80000)
.equ _FIQ_STACK, ( _IRQ_STACK - 0x80000)
.equ _ABT_STACK, ( _FIQ_STACK - 0x80000)
.equ _UND_STACK, ( _ABT_STACK - 0x80000)
.equ _SYS_STACK, ( _UND_STACK - 0x80000)


.macro CHANGE_TO_SVC
	msr cpsr_c, #(DISABLE_FIQ | DISABLE_IRQ | SVC_MOD)
.endm

.macro CHANGE_TO_SYS
	msr cpsr_c, #(DISABLE_IRQ | DISABLE_FIQ | SYS_MOD)
.endm

.macro CHANGE_TO_IRQ
	msr cpsr_c, #(DISABLE_IRQ | DISABLE_FIQ | IRQ_MOD)
.endm

.text
.global _start
@*****************     
@ 异常向量表  
@*****************
Reset:
	b Reset_Handle
Undefine:
	nop
SWI:
	b SWI_Handle
PrefetchAbort:
	nop
DataAbort:
	nop
Reserver:
	nop
IRQ:
	b IRQ_Handle
FIQ:
	nop

Reset_Handle:
    ldr sp, =4096                       @ 设置栈指针，以下都是C函数，调用前需要设好栈 SRAM是4K 所以这里是4096
    bl  disable_watch_dog               @ 关闭WATCHDOG，否则CPU会不断重启
    bl  Clock_Init
    bl  memsetup                        @ 设置存储控制器以使用SDRAM
    bl  nand_init               @初始化NAND Flash

    ldr     r0,     =0x30004000    @1.目标地址=0x30004000，这是SDRAM的的16KB地址处，前16KB用来存放MMU的一级页表4K*4 = 16K，一个页表项对应1M内存
    mov     r1,     #4096           @2.  源地址   = 4096，连接的时候，main.c中的代码都存在NAND Flash地址4096开始处
    mov     r2,     #0x00001000          @3.  复制长度 根据不同程序的大小进行修改
    bl      nand_read               @调用C函数nand_read  将NANDFALSH的4096地址后的2K内容拷贝到SDRAM的0x30004000处

    bl Mode_CPSR_SP_Init                 @ 重设栈指针向SDRAM顶端0x34000000 64M内存 为每个模式分配512KB大小的栈空间 开启MMU后需要将
					@SDRAM地址需要映射到原物理地址0X30000000，地址不变是因为SP初始化为物理地址
   @ bl  create_page_table               @ 设置页表 需要将开启MMU后需要用到的物理地址全部都映射
    @bl  mmu_init                        @ 启动MMU 开启前使用物理地址 开启后使用地址全部是虚拟地址
      
  
    ldr lr, =halt_loop
    ldr pc, =main                 @ 跳到SDRAM中继续执行第二部分代码  main.o在mmu.lds里面是连接到B0004000去的: 
				  @所以这里使用ldr pc,=main相当于pc = b0004000 + main函数的相对偏移 
				 @这里是硬件复位启动时将头4K NANDFLASH的内容拷贝到SRAM 然后将其第二部分代码拷贝到SDRAM，第一部分因为不需要载执行
				  @所以无需拷贝但是,所以mmu.lds时将第一部分连接到0地址，第二部分代码连接到B0004000，SDRAM中页表16KB空间后面
				  @程序连接到哪里不一定要下载到哪里，可以在启动的时候由启动文件复制到连接的位置。
halt_loop:
    b   halt_loop

Mode_CPSR_SP_Init:
	msr cpsr_c, #(DISABLE_IRQ | DISABLE_FIQ | IRQ_MOD)
	ldr sp,=0x32010000

	msr cpsr_c, #(DISABLE_IRQ | DISABLE_FIQ | FIQ_MOD)
	ldr sp,=0x32020000

	msr cpsr_c, #(DISABLE_IRQ | DISABLE_FIQ | ABT_MOD)
	ldr sp,=0x32030000
	
	msr cpsr_c, #(DISABLE_IRQ | DISABLE_FIQ | UND_MOD)
	ldr sp,=0x32040000

	msr cpsr_c, #(DISABLE_IRQ | DISABLE_FIQ | SYS_MOD)
	ldr sp,=0x32050000
	
	msr cpsr_c, #( DISABLE_FIQ | SVC_MOD) @开启IRQ中断
	ldr sp,=0x32060000   
	@管理模式应该方最后 或者设置完后应转化为管理模式 ，因为系统复位是进入管理模式
	@指令BL Mode_CPSR_SP_Init 将返回地址存在了R14_SVC 如果是其他模式没办法正确返回
	mov pc, lr


@记得这里在中断开始的时候关闭中断使能，在进入Dealwith_IRQ的时候开启
@这里使用从IRQ模式切换到SVC模式实现中断嵌套
IRQ_Handle: 
	sub lr,lr, #4	
	str lr,[r13,#-0x4]
	mrs lr,spsr
	str lr,[r13,#-0x8]
	str r0, [r13,#-0xc]
	mov r0, r13
	CHANGE_TO_SVC
	str lr, [r13,#-0x8]
	ldr lr, [r0, #-0x4]
	str lr, [r13,#-0x4]
	ldr lr, [r0 ,#-0x8]
	ldr r0, [r0 ,#-0xc]

	sub r13,r13, #8	        

	stmdb r13!,{r0-r12,lr}
	
	ldr lr, =Return_IRQ
	ldr pc, =Dealwith_IRQ
Return_IRQ:
	ldmia r13!,{r0-r12,lr}
	msr spsr, lr
	ldmfd r13!, {lr,pc}^  




SWI_Handle:
	nop

