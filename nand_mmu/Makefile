src1 := $(shell ls  *.c)
src2 := $(shell ls *.S)
src := $(src1) $(src2)
obj := $(patsubst %.c,%.o,$(src1))
obj := $(obj) $(patsubst %.S,%.o,$(src2))


nand_mmu.bin:$(obj)
	arm-linux-ld -Tnand_mmu.lds -o nand_mmu_elf $^
	arm-linux-objcopy -O binary -S nand_mmu_elf $@
	arm-linux-objdump -D -m arm nand_mmu_elf > nand_mmu.dis

%.o:%.S
	arm-linux-gcc $^  -c -Wall -O2 -o  $@
%.o:%.c 
	arm-linux-gcc $^  -c -Wall -O2 -o  $@ 
clean:
	rm -f nand_mmu_elf *~ *.o nand_mmu.bin nand_mmu.dis

