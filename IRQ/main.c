
#include"s3c24xx.h"


int main(void)
{
	LED_Init();
	//KEY_Init();
	Create_Page_Table();
	Start_mmu();
	EINT0_2_11_init();
	while(1);
	return 1;
}

