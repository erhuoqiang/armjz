SECTIONS { 
  firtst    0x00000000 : { head.o init.o nand.o}
  second    0xB0004000 : AT(4096) { main.o }
} 
 
