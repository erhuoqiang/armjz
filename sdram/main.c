//2440 的3个led的灯  和外部中断0 2 对应的按钮 控制寄存器和数据寄存器和上拉寄存器
#define GPFCON (*(volatile unsigned long *)0x56000050)  
#define GPFDAT (*(volatile unsigned long *)0x56000054)	  
#define GPFUP  (*(volatile unsigned long *)0x56000058)	  
//KEY3 和 KEY4 所在寄存器
#define GPGCON (*(volatile unsigned long *)0x56000060)  
#define GPGDAT (*(volatile unsigned long *)0x56000064)	  
#define GPGUP  (*(volatile unsigned long *)0x56000068)	  
//LED ON OFF
#define LED1_ON (GPFDAT &= ~(0x00000010))  //GPFDAT:4 = 0
#define LED2_ON (GPFDAT &= ~(0x00000020))  //GPFDAT:5 = 0
#define LED3_ON (GPFDAT &= ~(0x00000040))  //GPFDAT:6 = 0
#define LED1_OFF (GPFDAT |= (0x00000010))  //GPFDAT:4 = 1
#define LED2_OFF (GPFDAT |= (0x00000020))  //GPFDAT:5 = 1
#define LED3_OFF (GPFDAT |= (0x00000040))  //GPFDAT:6 = 1
#define LED1_REVERSAL (GPFDAT ^= (0x00000010))  //GPFDAT:4 = 1 //反转
#define LED2_REVERSAL (GPFDAT ^= (0x00000020))  //GPFDAT:5 = 1
#define LED3_REVERSAL (GPFDAT ^= (0x00000040))  //GPFDAT:6 = 1
//KEY JUDGE
#define KEY1_JUDGE ((GPFDAT & 0x00000001) == 0) //按键1是否按下
#define KEY2_JUDGE ((GPFDAT & 0x00000004) == 0) //按键2是否按下
#define KEY3_JUDGE ((GPGDAT & 0x00000008) == 0) //按键3是否按下
#define KEY4_JUDGE ((GPGDAT & 0x00000800) == 0) //按键4是否按下

void LED_Init(void)
{
	GPFCON &= ~(3<<8 | 3<<10 | 3<<12); //先将3个led的控制位都置0变为输入模式
	GPFCON |= 0x00001500;  //然后将其3个led控制位变成输出模式
	GPFDAT |= (1<<4 | 1<< 5 | 1<<6); //LED默认关闭状态
}

void KEY_Init(void)
{
	GPFCON &= ~(3<<0 | 3<<4); //KEY1和KEY2 键位输入模式
	GPFUP  &= ~(1<<0 | 1<<2); //输入模式  开启内部上拉 按键没有按下则为高电平 按下为低电平
	GPGCON &= ~(3<<6 | 3<<22); //KEY3和KEY4 键位输入模式
	GPGUP  &= ~(1<<3 | 1<<11); //输入模式  开启内部上拉 按键没有按下则为高电平 按下为低电平
}	
//因为Makefile中 编译采用了-O2优化 对于延时函数的变量需要加上 
//volatile修饰  这样才不会让编译器将延时优化掉
void Delay(volatile unsigned long count) 
{
	volatile int i = 0;
	for(;count>0;count--)
	for(i = 0; i < 100;i++);	
}


int main(void)
{
	LED_Init();
	KEY_Init();
	while(1)
	{
		if(KEY1_JUDGE)
		{	
			Delay(200);
			while(KEY1_JUDGE);
			LED1_REVERSAL;
		}
		if(KEY2_JUDGE)
		{
			Delay(200);
			while(KEY2_JUDGE);
			LED2_REVERSAL;
		}	
		if(KEY3_JUDGE)
		{
			Delay(200);
			while(KEY3_JUDGE);
			LED3_REVERSAL;
		}
		if(KEY4_JUDGE)
		{
			Delay(200);
			while(KEY4_JUDGE);
			LED1_REVERSAL;
			LED2_REVERSAL;
			LED3_REVERSAL;
		}
		
	}
	return 1;
}
