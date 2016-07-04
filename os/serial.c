#include "s3c24xx.h"
#include "include.h"

/*****************UART0******************/
#define TXD0READY   (1<<2)
#define RXD0READY   (1)

#define PCLK            50000000    // Clock_Init��������PCLKΪ50MHz
#define UART_CLK        PCLK        //  UART0��ʱ��Դ��ΪPCLK
#define UART_BAUD_RATE  115200      // ������115200
#define UART_BRD        ((int)((double)UART_CLK  / (UART_BAUD_RATE * 16)+0.5)- 1)

/* ��ʼ��UART0115200,8N1,������*/
void UART0_Init(void)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3����TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3�ڲ�����

    ULCON0  = 0x03;     // 8N1(8������λ���޽��飬1��ֹͣλ)
    UCON0   = 0x05;     // ��ѯ��ʽ��UARTʱ��ԴΪPCLK
    UFCON0  = 0x00;     // ��ʹ��FIFO
    UMCON0  = 0x00;     // ��ʹ������
    UBRDIV0 = UART_BRD; // ������Ϊ115200
}
/*UART0�����ж�ʹ�� */
void UART0_RX_Enable(void)
{        
   // UART0
     INTSUBMSK &= (~(1));  //����RX0���ж�
    INTMSK   &= (~(1<<28)); //����UART0�ж�
}


/*
char * Int_to_Str(int value)
{
	static char num[15] = "";
	unsigned int  i = 0;
	char * start = num;
	char * end = NULL;
	
	if(value == 0)
	{
		num[0] = 48;
		num[1] = 0;
		return num;
	}
	else if(value < 0)
	{
		value = 0 - value;
		num[i++]='-';
		start = num+1;
	}
	
	while(value)
	{
		num[i++] = value % 10 + 48;
		value /= 10;
	}
	num[i] = 0;
	end = num + i - 1;
	while(start < end)
	{
		*start ^= *end;	
		*end ^= *start;
		*start++ ^= *end--;
	}
	return num;
}
	*/
/* ����һ���ַ� */
void putc(unsigned char c)
{
    /* �ȴ���ֱ�����ͻ������е������Ѿ�ȫ�����ͳ�ȥ */
    while (!(UTRSTAT0 & TXD0READY));
    
    /* ��UTXH0�Ĵ�����д�����ݣ�UART���Զ��������ͳ�ȥ */
    UTXH0 = c;
}


/*�����ַ�*/
unsigned char getc(void)
{
    /* �ȴ���ֱ�����ջ������е������� */
    while (!(UTRSTAT0 & RXD0READY));
    
    /* ֱ�Ӷ�ȡURXH0�Ĵ��������ɻ�ý��յ������� */
    return URXH0;
}

char * SendStr_VIR( char * num)
{
	if(num == NULL)
		return NULL;
	while(*num)
	{
		putc(*num++);
	}
	return 	num;
}


/* �ж�һ���ַ��Ƿ�����*/
int isDigit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;       
}

/*�ж�һ���ַ��Ƿ�Ӣ����ĸ*/
int isLetter(unsigned char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    else if (c >= 'A' && c <= 'Z')
        return 1;       
    else
        return 0;
}
