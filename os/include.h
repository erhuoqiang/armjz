//#include"list.h"
#define NULL ((void *)(0))
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

char * Int_to_Str(int value);
char * SendStr_VIR( char * num);

/*mmu*/
void Create_Page_Table(void);
void Start_mmu(void);

/*mem*/
extern void init_sys_mmu(void);
extern void start_mmu(void);
extern void Enable_IRQ(void);
extern void umask_int(unsigned int offset);
extern void * get_free_pages(unsigned int flag, int order);
extern void put_free_pages(void * addr, int order);
extern char * INT_to_Str(unsigned int temp);
extern void init_page_map(void);
extern int kmalloc_init(void);
extern void * kmalloc( unsigned int size);
extern void kfree(void * addr);
extern int do_fork(int (*f)(void *),void *args);
extern void task_init();


