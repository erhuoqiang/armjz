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
    PRIORITY = (PRIORITY & ((~0x01) | (0x3<<7))) | (0x0 << 7) ;

    // EINT0、EINT2、EINT8_23使能
    INTMSK   &= (~(1<<0)) & (~(1<<2)) & (~(1<<5));
}

//因为Makefile中 编译采用了-O2优化 对于延时函数的变量需要加上 
//volatile修饰  这样才不会让编译器将延时优化掉
void Delay(volatile unsigned long count) 
{
	volatile int i = 0;
	for(;count>0;count--)
	for(i = 0; i < 100;i++);	
}
/***********LED初始化*********/
void LED_Init(void)
{
	GPFCON &= ~(3<<8 | 3<<10 | 3<<12); //先将3个led的控制位都置0变为输入模式
	GPFCON |= 0x00001500;  //然后将其3个led控制位变成输出模式
	GPFDAT |= (1<<4 | 1<< 5 | 1<<6); //LED默认关闭状态
}
/*********** key1-4 初始化*******/
void KEY_Init(void)
{
	Delay(2000);
	GPFCON &= ~(3<<0 | 3<<4); //KEY1和KEY2 键位输入模式
	GPFUP  &= ~(1<<0 | 1<<2); //输入模式  开启内部上拉 按键没有按下则为高电平 按下为低电平
	GPGCON &= ~(3<<6 | 3<<22); //KEY3和KEY4 键位输入模式
	GPGUP  &= ~(1<<3 | 1<<11); //输入模式  开启内部上拉 按键没有按下则为高电平 按下为低电平
}

/*用于测试程序是否启动成常使用*/
void test(void)
{
	while(1);
	{
		if(KEY1_JUDGE)
		{	
			Delay(2000);
			while(KEY1_JUDGE);
			LED1_REVERSAL;
		}
		if(KEY2_JUDGE)
		{
			Delay(2000);
			while(KEY2_JUDGE);
			LED2_REVERSAL;
		}	
		if(KEY3_JUDGE)
		{
			Delay(2000);
			while(KEY3_JUDGE);
			LED3_REVERSAL;
		}
		if(KEY4_JUDGE)
		{
			Delay(2000);
			while(KEY4_JUDGE);
			LED1_REVERSAL;
			LED2_REVERSAL;
			LED3_REVERSAL;
		}
		
	}
}

