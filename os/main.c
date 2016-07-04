#include"s3c24xx.h"
#include "include.h"

int test_process1(void *p)
{
	while(1)
	{
		Delay(2000);
		SendStr_VIR("test process id is:1\n\r");
	}
}
int test_process2(void *p)
{
	while(1)
	{
		Delay(2000);
		SendStr_VIR("test process id is:2\n\r");
	}
}
int main(void)
{ 
	LED_Init();
	//KEY_Init();
	UART0_Init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)
	Create_Page_Table();
	Start_mmu();
	EINT0_2_11_init();
	Timer0_Init();

	init_page_map();  //buddy算法 分配
	kmalloc_init();  //动态内存slab管理初始化
	task_init();  //多进程初始化

	EINT0_2_11_Enable();
	Timer0_IRQ_Enable();
	UART0_RX_Enable();
	 

	
	Delay(2000);
	SendStr_VIR("Init OK Into ^_^ OS\n\r");
	
	do_fork(test_process1, (void *)0x1);
	do_fork(test_process2, (void *)0x2);
	while(1)
	{
		Delay(2000);
		SendStr_VIR("this is the original process\n\r");
	}
	while(1);
	return 1;
}

