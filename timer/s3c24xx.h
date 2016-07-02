/********MMU开启后映射的虚拟地址 main函数以后的都是用虚拟地址***********************/
//2440 的3个led的灯  和外部中断0 2 对应的按钮 控制寄存器和数据寄存器和上拉寄存器
#define GPFCON (*(volatile unsigned long *)0x56000050)  
#define GPFDAT (*(volatile unsigned long *)0x56000054)	  
#define GPFUP  (*(volatile unsigned long *)0x56000058)	  
//KEY3 和 KEY4 所在寄存器
#define GPGCON (*(volatile unsigned long *)0x56000060)  
#define GPGDAT (*(volatile unsigned long *)0x56000064)	  
#define GPGUP  (*(volatile unsigned long *)0x56000068)	  
//LED ON OFF
#define LED1_ON (GPFDAT &= ~(0x00000010))  //GPFDAT:4 = 0
#define LED2_ON (GPFDAT &= ~(0x00000020))  //GPFDAT:5 = 0
#define LED3_ON (GPFDAT &= ~(0x00000040))  //GPFDAT:6 = 0
#define LED1_OFF (GPFDAT |= (0x00000010))  //GPFDAT:4 = 1
#define LED2_OFF (GPFDAT |= (0x00000020))  //GPFDAT:5 = 1
#define LED3_OFF (GPFDAT |= (0x00000040))  //GPFDAT:6 = 1
#define LED1_REVERSAL (GPFDAT ^= (0x00000010))  //GPFDAT:4 = 1 //反转
#define LED2_REVERSAL (GPFDAT ^= (0x00000020))  //GPFDAT:5 = 1
#define LED3_REVERSAL (GPFDAT ^= (0x00000040))  //GPFDAT:6 = 1
//KEY JUDGE
#define KEY1_JUDGE ((GPFDAT & 0x00000001) == 0) //按键1是否按下
#define KEY2_JUDGE ((GPFDAT & 0x00000004) == 0) //按键2是否按下
#define KEY3_JUDGE ((GPGDAT & 0x00000008) == 0) //按键3是否按下
#define KEY4_JUDGE ((GPGDAT & 0x00000800) == 0) //按键4是否按下
/*GPBCON Tout0-3 PWM*/
#define GPBCON              (*(volatile unsigned long *)0x56000010)
#define GPBDAT              (*(volatile unsigned long *)0x56000014)

/*interrupt registes*/
#define SRCPND              (*(volatile unsigned long *)0x4A000000)
#define INTMOD              (*(volatile unsigned long *)0x4A000004)
#define INTMSK              (*(volatile unsigned long *)0x4A000008)
#define PRIORITY            (*(volatile unsigned long *)0x4A00000c)
#define INTPND              (*(volatile unsigned long *)0x4A000010)
#define INTOFFSET           (*(volatile unsigned long *)0x4A000014)
#define SUBSRCPND           (*(volatile unsigned long *)0x4A000018)
#define INTSUBMSK           (*(volatile unsigned long *)0x4A00001c)
/********MMU开启前的物理地址***********************/
/* WOTCH DOG register */
#define     WTCON           (*(volatile unsigned long *)0x53000000)

/* SDRAM regisers */
#define     MEM_CTL_BASE    0x48000000
#define     SDRAM_BASE      0x30000000

/* NAND Flash registers */
/*#define NFCONF              (*(volatile unsigned int  *)0x4e000000)
#define NFCMD               (*(volatile unsigned char *)0x4e000004)
#define NFADDR              (*(volatile unsigned char *)0x4e000008)
#define NFDATA              (*(volatile unsigned char *)0x4e00000c)
#define NFSTAT              (*(volatile unsigned char *)0x4e000010)
*/
/*GPIO registers*/
/*#define GPBCON              (*(volatile unsigned long *)0x56000010)
#define GPBDAT              (*(volatile unsigned long *)0x56000014)

#define GPFCON              (*(volatile unsigned long *)0x56000050)
#define GPFDAT              (*(volatile unsigned long *)0x56000054)
#define GPFUP               (*(volatile unsigned long *)0x56000058)

#define GPGCON              (*(volatile unsigned long *)0x56000060)
#define GPGDAT              (*(volatile unsigned long *)0x56000064)
#define GPGUP               (*(volatile unsigned long *)0x56000068)

#define GPHCON              (*(volatile unsigned long *)0x56000070)
#define GPHDAT              (*(volatile unsigned long *)0x56000074)
#define GPHUP               (*(volatile unsigned long *)0x56000078)

*/

 /*UART registers*/
#define ULCON0              (*(volatile unsigned long *)0x50000000)
#define UCON0               (*(volatile unsigned long *)0x50000004)
#define UFCON0              (*(volatile unsigned long *)0x50000008)
#define UMCON0              (*(volatile unsigned long *)0x5000000c)
#define UTRSTAT0            (*(volatile unsigned long *)0x50000010)
#define UTXH0               (*(volatile unsigned char *)0x50000020)
#define URXH0               (*(volatile unsigned char *)0x50000024)
#define UBRDIV0             (*(volatile unsigned long *)0x50000028)



/*external interrupt registers*/
#define EINTMASK            (*(volatile unsigned long *)0x560000a4)
#define EINTPEND            (*(volatile unsigned long *)0x560000a8)

/*clock registers*/
#define	LOCKTIME		(*(volatile unsigned long *)0x4c000000)
#define	MPLLCON		(*(volatile unsigned long *)0x4c000004)
#define	UPLLCON		(*(volatile unsigned long *)0x4c000008)
#define	CLKCON		(*(volatile unsigned long *)0x4c00000c)
#define	CLKSLOW		(*(volatile unsigned long *)0x4c000010)
#define	CLKDIVN		(*(volatile unsigned long *)0x4c000014)


/*PWM & Timer registers*/
#define	TCFG0		(*(volatile unsigned long *)0x51000000)
#define	TCFG1		(*(volatile unsigned long *)0x51000004)
#define	TCON		(*(volatile unsigned long *)0x51000008)
#define	TCNTB0		(*(volatile unsigned long *)0x5100000c)
#define	TCMPB0		(*(volatile unsigned long *)0x51000010)
#define	TCNTO0		(*(volatile unsigned long *)0x51000014)

#define GSTATUS1    (*(volatile unsigned long *)0x560000B0)
