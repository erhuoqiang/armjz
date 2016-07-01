#include"s3c24xx.h"
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
    unsigned long val;

    switch( oft )
    {
        // S2被按下
        case 0: 
        {   
            GPFDAT |= (0x7<<4);   // 所有LED熄灭
            GPFDAT &= ~(1<<6);      // LED1点亮
            break;
        }
        
        // S3被按下
        case 2:
        {   
            GPFDAT |= (0x7<<4);   // 所有LED熄灭
            GPFDAT &= ~(1<<5);      // LED2点亮
            break;
        }

        // K4被按下
        case 5:
        {   
            GPFDAT |= (0x7<<4);   // 所有LED熄灭
            GPFDAT &= ~(1<<4);      // LED4点亮                
            break;
        }

        default:
            break;
    }

    //清中断
    if( oft == 5 ) 
        EINTPEND = (1<<11);   // EINT8_23合用IRQ5
    SRCPND = 1<<oft;
    INTPND = 1<<oft; Enable_IRQ();
	
}
