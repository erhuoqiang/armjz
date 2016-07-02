#include"s3c24xx.h"

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

