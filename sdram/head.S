@2440启动文件的作用 硬件初始化和软件初始化
@硬件初始化 包括 1看门狗初始化  2SDRAN初始化  3系统时钟初始化
@软件初始化 包括 1设置栈，函数返回地址  2跳转到main函数执行 3执行完main函数后的清理工作
@系统始终采用系统复位后默认的时钟 所以也无须设置
.equ SDRAM_BASE, 0x3000000   //bank6 对应sdram
.equ MEM_CTL_BASE, 0x48000000  //0x4800 0000 - 0x5fff ffff是内部寄存器的空间

.text
.global _start
_start:
	bl disable_watch_dog	@关闭看门狗
	bl mem_ctl_init  @初始化设置储存控制器
	bl copy_sram_to_sdram @拷贝sram 4K空间到sdram
	
	ldr pc,=sdram_on  @跳转到sdram中的sdram_on执行。NAND flash在系统启动的时候把头4k程序拷贝到sram，也就是说，当前是在sram中执行程序，arm-linux-ld链接的时候指定-Ttext 0x3000 0000,代表代码段在0x30000000开始，也就是程序第一条指令连接地址是0x3000 0000（但是这不代表程序在下载的时候要下载到这个链接地址，下载程序都是先下到nand flash在从nand flash拷贝到连接地址去运行的(这段拷贝的程序是放在sram中执行，因为nand flash上面不能执行程序),而这里因为程序小于4k可以直接从sram拷贝到sdram中）,伪指令ldr pc,=sdram_on 是将30000000+sdram_on的偏移量的值给pc，所以可以实现跳转,而bl ,b,是记录下当正在前执行程序的地址的下一条指令的地址，也就是lr存放的地址还是在sram中，所以用mov pc,lr时不会跳转 还是在sram中，addr和adr伪指令 将标签对应当前pc的相对地址给寄存器，所以也是在sram中 他们都是用相对地址，没有把链接地址加进去，所不没办法实现跳转

sdram_on:
	ldr sp, =0x34000000 @设置栈顶为0x3000 0000 + 64M
	bl main
loop:
	b loop;




disable_watch_dog:
	  @ 往WATCHDOG寄存器写0即可
	mov r1, #0x53000000  @将看门狗寄存器地址给r1
	mov r2, #0x0
	str r2, [r1]  @[r1] = r2
	mov pc, lr   @ARM的三级流水线这里不用lr-4，只有进入中断的时候才需要考虑，而这里是使用bl 指令自动将下一条指令地址存入也就是pc - 4 直接给lr




copy_sram_to_sdram:
   @ 将Steppingstone的4K数据全部复制到SDRAM中去
    @ Steppingstone起始地址为0x00000000，SDRAM中起始地址为0x30000000
    
    mov r1, #0 @sram的起始地址给r1
    ldr r2, =SDRAM_BASE
    mov r3, #4*1024
LL: 
    ldr r4, [r1],#4     @ 从Steppingstone读取4字节的数据，并让源地址加4
    str r4, [r2],#4     @ 将此4字节的数据复制到SDRAM中，并让目地地址加4
    cmp r1, r3          @ 判断是否完成：源地址等于Steppingstone的未地址？
    bne LL             @ 若没有复制完，继续
    mov pc,     lr      @ 返回




mem_ctl_init:
	    @ 设置存储控制器以便使用SDRAM等外设
	mov r1, #MEM_CTL_BASE  @将储存控制寄存器组的基地址给r1
	adrl r2, mem_cfg_val  @将mem_cfg_val存放寄存器配置的值的空间起始地址给r2，adr和adrl伪指令是将基于PC相对偏移的地址读取到寄存器的与伪指令ldr不同，ldr是直接把32位的地址置寄存器，不是偏移量，所ldr可以用来从SRAM跳转SDRAM前提是编译arm-linux-ld -Ttext 0x3000 0000
	add r3, r1, #52  @因为需要设置13个寄存器  r3  = r1 + 13 * 4
L:
	ldr r4, [r2], #4 
	str r4, [r1], #4
	cmp r1,r3
	bne L
	mov pc, lr

.align 4  @4字对齐
mem_cfg_val:
    @ 存储控制器13个寄存器的设置值  ,具体值请看用户手册和韦东山的书
    .long   0x22011110      @ BWSCON
    .long   0x00000700      @ BANKCON0
    .long   0x00000700      @ BANKCON1
    .long   0x00000700      @ BANKCON2
    .long   0x00000700      @ BANKCON3  
    .long   0x00000700      @ BANKCON4
    .long   0x00000700      @ BANKCON5
    .long   0x00018005      @ BANKCON6
    .long   0x00018005      @ BANKCON7
    .long   0x008C07A3      @ REFRESH
    .long   0x000000B1      @ BANKSIZE
    .long   0x00000030      @ MRSRB6
    .long   0x00000030      @ MRSRB7	



