#define MMU_FULL_ACCESS     (3 << 10)   /* 访问权限 */
#define MMU_DOMAIN          (0 << 5)    /* 属于哪个域 */
#define MMU_SPECIAL         (1 << 4)    /* 必须是1 */
#define MMU_CACHEABLE       (1 << 3)    /* cacheable */
#define MMU_BUFFERABLE      (1 << 2)    /* bufferable */
#define MMU_SECTION         (2)         /* 表示这是段描述符 */
#define MMU_SECDESC         (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | \
                             MMU_SECTION)
#define MMU_SECDESC_WB      (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | \
                             MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)
#define MMU_SECTION_SIZE    0x00100000

/*
 * 设置页表
 */
void Create_Page_Table(void)
{

/* 
 * 用于段描述符的一些宏定义
 */ 


    unsigned long virtuladdr, physicaladdr;
    unsigned long *mmu_tlb_base = (unsigned long *)0x30000000;
    
    /*
     * Steppingstone的起始物理地址为0，第一部分程序的起始运行地址也是0，
     * 为了在开启MMU后仍能运行第一部分的程序，
     * 将0～1M的虚拟地址映射到同样的物理地址
     */
    virtuladdr = 0;
    physicaladdr = 0;
    *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                            MMU_SECDESC_WB;
	/**********各个模式的栈初始化时候可以用到***********************/
   virtuladdr = 0x30000000;
   physicaladdr = 0x30000000;
    while (virtuladdr <= 0x34000000)
    {
        *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                                MMU_SECDESC_WB;  //页表基地址加上虚拟地址高12位的值左移两位，这里没有左移是因为页表
								//基地址是int * 指针 +1等于+4就相当于左移两位了
        virtuladdr += 0x100000;
        physicaladdr += 0x100000;
    }
     /************中断寄存器物理地址到虚拟地址的映射**************/
    virtuladdr = 0x4A000000;
    physicaladdr = 0x4A000000;
    *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                            MMU_SECDESC_WB;
    /*
     * 0x56000000是GPIO寄存器的起始物理地址，
     * GPBCON和GPBDAT这两个寄存器的物理地址0x56000050、0x56000054，
     * 为了在第二部分程序中能以地址0xA0000050、0xA0000054来操作GPFCON、GPFDAT，
     * 把从0xA0000000开始的1M虚拟地址空间映射到从0x56000000开始的1M物理地址空间
     */
    virtuladdr = 0x56000000;
    physicaladdr = 0x56000000;
    *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                            MMU_SECDESC;

    /*
     * SDRAM的物理地址范围是0x30000000～0x33FFFFFF，
     * 将虚拟地址0xB0000000～0xB3FFFFFF映射到物理地址0x30000000～0x33FFFFFF上，
     * 总共64M，涉及64个段描述符
     */
    virtuladdr = 0x30000000;
    physicaladdr = 0x30000000;
    while (virtuladdr < 0xB4000000)
    {
        *(mmu_tlb_base + (virtuladdr >> 20)) = (physicaladdr & 0xFFF00000) | \
                                                MMU_SECDESC_WB;  //页表基地址加上虚拟地址高12位的值左移两位，这里没有左移是因为页表
								//基地址是int * 指针 +1等于+4就相当于左移两位了
        virtuladdr += 0x100000;
        physicaladdr += 0x100000;
    }
}

/*
 * 启动MMU
 */
void Start_mmu(void)
{
    unsigned long ttb = 0x30000000;  /*页表基地址放这里*/

	asm(//代码列表
	"mcr p15, 0, %0, c2, c0, 0\n"  //将页表的基地址L1_PTR_BASE_ADDR给CP15：C2保存  C2是CP15Y用来保存页面基地址的寄存器 （%0代表输入运算符的第一个，则是上面定义的ttb)
	"mvn r0, #0\n"   //mvn是 先将目的操作数取反 在给源操作数 ，r0 = ~0 = 0xffffffff 
	"mcr p15, 0, r0, c3, c0, 0\n"  //r0 给 CP15：C3寄存器，CP15：C3寄存器是控制内存域权限的 全部为1  则 可读写 不受AP控制
	"mov r0, #0x1\n"      //r0 = 0x1
	"mcr p15, 0, r0, c1, c0, 0\n"  //r0 =0x1 给 CP15：C1寄存器的第一位CP15：C1寄存器的第一位，其掌握了激活MMU的大全 也就是该位置1，MMU激活
				//后面运行的程序的地址都要是虚拟地址（前面的程序则用的是物理地址)虚拟地址到物理地址的转化由CP15协处理器控制。
					// init_sys_mmu（）的作用是建立页表 也就是初始化页表的每一项，而CP15协处理器则负责接受虚拟地址将其高12位
					//左移18位和页表基地址相加 找到对应的页表项 读取后 取高12 位和虚拟地址的低20位结合则完成转换
	"mov r0, r0\n"  //可忽略
	"mov r0, r0\n"
	"mov r0, r0\n"
	:		//输出运算符列表 
	: "r" (ttb)	//输入运算符列表      把ttb作为输入参数， 给通用寄存器，r 代表通用寄存器R0-R15
	: "r0"    	//被更改资源列表 代码列表中 只有 r0被更改了
	);
}
