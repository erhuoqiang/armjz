#include"s3c24xx.h"
/*����ȫ��IRQ�ж�*/
void Enable_IRQ(void)
{
	//SendStr_VIR("Enable_IRQ\n");
	asm volatile(
	"mrs r4,cpsr\n\t"
	"bic r4,r4,#0x80\n\t"    //bitָ���ǽ���Ӧλ��0  �����泣�� Ϊ1��λ��0
	"msr cpsr,r4\n\t"
	:::"r4");
	
}
/*�ر�ȫ��IRQ�ж�*/
void Disable_IRQ(void)
{
	
	asm volatile(
	"mrs r4,cpsr\n\t"
	"orr r4,r4,#0x80 \n\t"  
	"msr cpsr,r4\n\t"
	:::"r4");
	//SendStr_VIR("Disable_IRQ\n");
}
/*IRQ�жϴ�����*/
void Dealwith_IRQ(void)
{
    volatile unsigned long oft = INTOFFSET;
    unsigned long val;

    switch( oft )
    {
        // S2������
        case 0: 
        {   
            GPFDAT |= (0x7<<4);   // ����LEDϨ��
            GPFDAT &= ~(1<<6);      // LED1����
            break;
        }
        
        // S3������
        case 2:
        {   
            GPFDAT |= (0x7<<4);   // ����LEDϨ��
            GPFDAT &= ~(1<<5);      // LED2����
            break;
        }

        // K4������
        case 5:
        {   
            GPFDAT |= (0x7<<4);   // ����LEDϨ��
            GPFDAT &= ~(1<<4);      // LED4����                
            break;
        }

        default:
            break;
    }

    //���ж�
    if( oft == 5 ) 
        EINTPEND = (1<<11);   // EINT8_23����IRQ5
    SRCPND = 1<<oft;
    INTPND = 1<<oft; Enable_IRQ();
	
}
