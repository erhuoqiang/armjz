all:
	arm-linux-gcc -c -o head.o  head.S
	arm-linux-gcc -c -o   main.o  main.c 
	arm-linux-ld -Tsdram.lds -g head.o main.o -o sdram_elf #这里注意.o文件的顺序
	arm-linux-objcopy -O binary -S sdram_elf sdram.bin
	arm-linux-objdump -D -m arm sdram_elf > sdram.dis
clean:
	rm -f sdram.bin  *.o  sdram_elf sdram.dis *~
	 
