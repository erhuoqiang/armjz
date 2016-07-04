SECTIONS { 
  firtst    0x00000000 : { startup.o ochip__perip_init.o nand.o   }
  second    0xB0004000 : AT(4096) { main.o peripheral_init.o mmu.o interrupt.o serial.o list.o mem.o proc.o task.o   }
} 
 
