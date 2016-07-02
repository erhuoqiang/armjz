
#include"s3c24xx.h"


int main(void)
{
	LED_Init();
	//KEY_Init();
	
	Create_Page_Table();
	Start_mmu();
	EINT0_2_11_init();
	EINT0_2_11_Enable();
	Timer0_Init();
	Timer0_IRQ_Enable();
	while(1);
	return 1;
}

