/*UART0*/
void UART0_Init(void);
void UART0_RX_Enable(void);
void putc(unsigned char c);
unsigned char getc(void);
int isDigit(unsigned char c);
int isLetter(unsigned char c);

/*onchip_peripheral*/
void disable_watch_dog(void);
void Clock_Init(void);

/*peripheral*/
void Delay(volatile unsigned long count);
void LED_Init(void);
void KEY_Init(void);

/*intterupt*/
void EINT0_2_11_init();
void EINT0_2_11_Enable(void);
void Timer0_Init(void);
void Timer0_IRQ_Enable(void);
void Enable_IRQ(void);
void Disable_IRQ(void);

/*mmu*/
void Create_Page_Table(void);
void Start_mmu(void);

