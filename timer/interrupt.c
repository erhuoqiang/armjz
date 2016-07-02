#include"s3c24xx.h"
/* LED1,LED2,LED4对应GPF4、GPF5、GPF6 设置位输出模式 */ 
#define	GPF4_out	(1<<(4*2))
#define	GPF5_out	(1<<(5*2))
#define	GPF6_out	(1<<(6*2))

#define	GPF4_msk	(3<<(4*2))
#define	GPF5_msk	(3<<(5*2))
#define	GPF6_msk	(3<<(6*2))

 /* S2,S3,S4对应GPF0、GPF2、GPG3的 外部中断模式 */
#define GPF0_eint     (0x2<<(0*2))
#define GPF2_eint     (0x2<<(2*2))
#define GPG3_eint     (0x2<<(3*2))

#define GPF0_msk    (3<<(0*2))
#define GPF2_msk    (3<<(2*2))
#define GPG3_msk    (3<<(3*2))


/* 初始化GPIO引脚为外部中断
 * GPIO引脚用作外部中断时，默认为低电平触发、IRQ方式(不用设置INTMOD)*/
void EINT0_2_11_init( )
{
    // S2,S3对应的2根引脚设为中断引脚 EINT0,ENT2
    GPFCON &= ~(GPF0_msk | GPF2_msk);
    GPFCON |= GPF0_eint | GPF2_eint;

    // S4对应的引脚设为中断引脚EINT11
    GPGCON &= ~GPG3_msk;
    GPGCON |= GPG3_eint;
    
    // 对于EINT11，需要在EINTMASK寄存器中使能它
    EINTMASK &= ~(1<<11);
        
    /* 设定优先级：ARB_SEL0 = 00b, ARB_MODE0 = 0: REQ1 > REQ3，即EINT0 > EINT2
     * 仲裁器1、6无需设置 最终： EINT0 > EINT2 > EINT11即K2 > K3 > K4*/
    PRIORITY = (PRIORITY & ((~0x01) | (0x3<<7))) | (0x1 << 19) ;
	
    
}

/* EINT0、EINT2、EINT8_23使能*/ 
void EINT0_2_11_Enable(void)
{        
   // EINT0、EINT2、EINT8_23使能
    INTMSK   &= (~(1<<0)) & (~(1<<2)) & (~(1<<5));
}

void Timer0_Init(void)
{
    TCFG0  = 99;        // 预分频器0 = 99        
    TCFG1  = 0x03;      // 选择16分频
    TCNTB0 = 31250;     // 0.5秒钟触发一次中断
    TCON   |= (1<<1);   // 手动更新 第一次使用时将TCNTB0的值写入到其内部寄存器进行计数，开启自动加载后每次都会将外部TCNTB0的值写入TCNTn中
			//TCMPBn同理   TCNTBn和TCMPBn决定PWM占空比，第一次改变TCNTB0后，后面如果要改变数值则直接修改TCNTB就可以，下次自动
			//装载的时候就会将新值加入
    TCON   = 0x09;      // 自动加载，清“手动更新”位，启动定时器0
}

/* 定时器0中断使能*/ 
void Timer0_IRQ_Enable(void)
{        
    // 定时器0中断使能
    INTMSK   &= (~(1<<10));
}

/*激活全局IRQ中断*/
void Enable_IRQ(void)
{
	//SendStr_VIR("Enable_IRQ\n");
	asm volatile(
	"mrs r4,cpsr\n\t"
	"bic r4,r4,#0x80\n\t"    //bit指令是将对应位置0  将后面常数 为1的位置0
	"msr cpsr,r4\n\t"
	:::"r4");
	
}

/*关闭全局IRQ中断*/
void Disable_IRQ(void)
{
	
	asm volatile(
	"mrs r4,cpsr\n\t"
	"orr r4,r4,#0x80 \n\t"  
	"msr cpsr,r4\n\t"
	:::"r4");
	//SendStr_VIR("Disable_IRQ\n");
}

/*IRQ中断处理函数*/
void Dealwith_IRQ(void)
{
    volatile unsigned long oft = INTOFFSET;
//    unsigned long val;
    //清中断
    if( oft == 5 ) 
        EINTPEND = (1<<11);   // EINT8_23合用IRQ5
    SRCPND = 1<<oft;  //对应位写1代表清中断
    INTPND = 1<<oft;   
    Enable_IRQ();  //先清除对应中断后 在使能中断 实现中断嵌套 要不然会一直进入中断，如果先使能的话，因为中断标志位还没清除所以又会进入中断
    switch( oft )
    {
        // S2被按下
        case 0:  //EINT0
        {   
            GPFDAT |= (0x7<<4);   // 所有LED熄灭
            GPFDAT &= ~(1<<6);      // LED1点亮
            break;
        }
        
        // S3被按下
        case 2:  //EINT2
        {   
            GPFDAT |= (0x7<<4);   // 所有LED熄灭
            GPFDAT &= ~(1<<5);      // LED2点亮
            break;
        }

        // K4被按下
        case 5: //EINT8_23
        {   
            GPFDAT |= (0x7<<4);   // 所有LED熄灭
            GPFDAT &= ~(1<<4);      // LED4点亮                
            break;
        }
	case 10:   //Timer0
	{
	    extern void Timer0_Handle();
	    Timer0_Handle();
	    break;
	}

        default:
            break;
    }
	
}
void Timer0_Handle()
{
	    LED1_REVERSAL;
	    LED2_REVERSAL;
	    LED3_REVERSAL;
}

