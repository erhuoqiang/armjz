
#include"s3c24xx.h"
#include "include.h"

int main(void)
{ 
	LED_Init();
	//KEY_Init();
	UART0_Init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)
	Create_Page_Table();
	Start_mmu();
	EINT0_2_11_init();
	EINT0_2_11_Enable();
	Timer0_Init();
	Timer0_IRQ_Enable();
	UART0_RX_Enable();
	while(1);
	return 1;
}

