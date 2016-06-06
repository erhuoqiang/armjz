#define GPF4CON (*(volatile unsigned long *)0x56000050)
#define GPF4DAT (*(volatile unsigned long *)0x56000054)

void delay(unsigned long count)
{
	int i = 0;
	for(;count>0;count--)
	for(i = 0; i < 100;i++);	
}
int main(void)
{
	
	GPF4CON = 0x00001500;	
	GPF4DAT = 0x00000000;
	while(1)
	{
		GPF4CON = 0x00000100;	
		GPF4DAT = 0x00000000;
		delay(1000);
		GPF4CON = 0x00000500;	
		GPF4DAT = 0x00000000;
		delay(1000);
		GPF4CON = 0x00001500;	
		GPF4DAT = 0x00000000;
		delay(1000);
	}
	return 1;
}
