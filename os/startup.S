@*************************************************************************
@ File��head.S
@ ���ܣ�����SDRAM����nandflash�ڶ����ִ��븴�Ƶ�SDRAM������ҳ��������MMU��
@       Ȼ������SDRAM����ִ��
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

@Ϊÿ��ģʽ����512KB��С��ջ�ռ�
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
@ �쳣������  
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
    ldr sp, =4096                       @ ����ջָ�룬���¶���C����������ǰ��Ҫ���ջ SRAM��4K ����������4096
    bl  disable_watch_dog               @ �ر�WATCHDOG������CPU�᲻������
    bl  Clock_Init
    bl  memsetup                        @ ���ô洢��������ʹ��SDRAM
    bl  nand_init               @��ʼ��NAND Flash

    ldr     r0,     =0x30004000    @1.Ŀ���ַ=0x30004000������SDRAM�ĵ�16KB��ַ����ǰ16KB�������MMU��һ��ҳ��4K*4 = 16K��һ��ҳ�����Ӧ1M�ڴ�
    mov     r1,     #4096           @2.  Դ��ַ   = 4096�����ӵ�ʱ��main.c�еĴ��붼����NAND Flash��ַ4096��ʼ��
    mov     r2,     #0x00008000          @3.  ���Ƴ��� ���ݲ�ͬ����Ĵ�С�����޸�
    bl      nand_read               @����C����nand_read  ��NANDFALSH��4096��ַ���2K���ݿ�����SDRAM��0x30004000��

    bl Mode_CPSR_SP_Init                 @ ����ջָ����SDRAM����0x34000000 64M�ڴ� Ϊÿ��ģʽ����512KB��С��ջ�ռ� ����MMU����Ҫ��
					@SDRAM��ַ��Ҫӳ�䵽ԭ������ַ0X30000000����ַ��������ΪSP��ʼ��Ϊ������ַ
   @ bl  create_page_table               @ ����ҳ�� ��Ҫ������MMU����Ҫ�õ���������ַȫ����ӳ��
    @bl  mmu_init                        @ ����MMU ����ǰʹ��������ַ ������ʹ�õ�ַȫ���������ַ
      
  
    ldr lr, =halt_loop
    ldr pc, =main                 @ ����SDRAM�м���ִ�еڶ����ִ���  main.o��mmu.lds���������ӵ�B0004000ȥ��: 
				  @��������ʹ��ldr pc,=main�൱��pc = b0004000 + main���������ƫ�� 
				 @������Ӳ����λ����ʱ��ͷ4K NANDFLASH�����ݿ�����SRAM Ȼ����ڶ����ִ��뿽����SDRAM����һ������Ϊ����Ҫ��ִ��
				  @�������追������,����mmu.ldsʱ����һ�������ӵ�0��ַ���ڶ����ִ������ӵ�B0004000��SDRAM��ҳ��16KB�ռ����
				  @�������ӵ����ﲻһ��Ҫ���ص����������������ʱ���������ļ����Ƶ����ӵ�λ�á�
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
	
	msr cpsr_c, #( DISABLE_FIQ | SVC_MOD) @����IRQ�ж�
	ldr sp,=0x32060000   
	@����ģʽӦ�÷���� �����������Ӧת��Ϊ����ģʽ ����Ϊϵͳ��λ�ǽ������ģʽ
	@ָ��BL Mode_CPSR_SP_Init �����ص�ַ������R14_SVC ���������ģʽû�취��ȷ����
	mov pc, lr


@�ǵ��������жϿ�ʼ��ʱ��ر��ж�ʹ�ܣ��ڽ���Dealwith_IRQ��ʱ����
@����ʹ�ô�IRQģʽ�л���SVCģʽʵ���ж�Ƕ��
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
