src1 := $(shell ls  *.c)
src2 := $(shell ls *.S)
src := $(src1) $(src2)
obj := $(patsubst %.c,%.o,$(src1))
obj := $(obj) $(patsubst %.S,%.o,$(src2))


os.bin:$(obj)
	arm-linux-ld -Tos.lds -o os_elf $^
	arm-linux-objcopy -O binary -S os_elf $@
	arm-linux-objdump -D -m arm os_elf > os.dis

%.o:%.S
	arm-linux-gcc $^  -c -Wall -O2 -o  $@
%.o:%.c 
	arm-linux-gcc $^  -c -Wall  -O2 -o  $@ 
clean:
	rm -f os_elf *~ *.o os.bin os.dis

