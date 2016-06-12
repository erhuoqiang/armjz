all:
	arm-linux-gcc -c -o crt0.o  crt0.S
	arm-linux-gcc -c -o   main.o  main.c 
	arm-linux-ld -Tkey_led.lds -g crt0.o main.o -o led_on_elf #这里注意.o文件的顺序
	arm-linux-objcopy -O binary -S led_on_elf led_on.bin
clean:
	rm -f led_on.bin  *.o  led_on_elf
	 
