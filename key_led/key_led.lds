SECTIONS
{
	. = 0x00000000;
	.text  : {*(.text)}
	.rodata ALIGN(4): {*(.rodata)}
	.data ALIGN(4): {*(.data)}
	.bss ALIGN(4) : {*(.bss) *(.COMMOM)}
}
