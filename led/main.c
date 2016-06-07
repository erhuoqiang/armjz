// 2440 的3个led的灯  和外部中断0 2 对应的按钮  
#define GPFCON (*(volatile unsigned long *)0x56000050)  
#define GPFDAT (*(volatile unsigned long *)0x56000054)	  



void delay(unsigned long count)
{
	int i = 0;
	for(;count>0;count--)
	for(i = 0; i < 100;i++);	
}
int main(void)
{
	
	GPFCON = 0x00001500;	
	GPFDAT = 0x00000000;
	while(1)
	{
		GPFCON = 0x00000100;	
		GPFDAT = 0x00000000;
		delay(1000);
		GPFCON = 0x00000500;	
		GPFDAT = 0x00000000;
		delay(1000);
		GPFCON = 0x00001500;	
		GPFDAT = 0x00000000;
		delay(1000);
	}
	return 1;
}
