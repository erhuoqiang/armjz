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
