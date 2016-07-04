#include "s3c24xx.h"
#include "include.h"

/*****************UART0******************/
#define TXD0READY   (1<<2)
#define RXD0READY   (1)

#define PCLK            50000000    // Clock_Init函数设置PCLK为50MHz
#define UART_CLK        PCLK        //  UART0的时钟源设为PCLK
#define UART_BAUD_RATE  115200      // 波特率115200
#define UART_BRD        ((int)((double)UART_CLK  / (UART_BAUD_RATE * 16)+0.5)- 1)

/* 初始化UART0115200,8N1,无流控*/
void UART0_Init(void)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3用作TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3内部上拉

    ULCON0  = 0x03;     // 8N1(8个数据位，无较验，1个停止位)
    UCON0   = 0x05;     // 查询方式，UART时钟源为PCLK
    UFCON0  = 0x00;     // 不使用FIFO
    UMCON0  = 0x00;     // 不使用流控
    UBRDIV0 = UART_BRD; // 波特率为115200
}
/*UART0接受中断使能 */
void UART0_RX_Enable(void)
{        
   // UART0
     INTSUBMSK &= (~(1));  //开启RX0子中断
    INTMSK   &= (~(1<<28)); //开启UART0中断
}

/* 发送一个字符 */
void putc(unsigned char c)
{
    /* 等待，直到发送缓冲区中的数据已经全部发送出去 */
    while (!(UTRSTAT0 & TXD0READY));
    
    /* 向UTXH0寄存器中写入数据，UART即自动将它发送出去 */
    UTXH0 = c;
}

/*接收字符*/
unsigned char getc(void)
{
    /* 等待，直到接收缓冲区中的有数据 */
    while (!(UTRSTAT0 & RXD0READY));
    
    /* 直接读取URXH0寄存器，即可获得接收到的数据 */
    return URXH0;
}

/* 判断一个字符是否数字*/
int isDigit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;       
}

/*判断一个字符是否英文字母*/
int isLetter(unsigned char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    else if (c >= 'A' && c <= 'Z')
        return 1;       
    else
        return 0;
}
