OUTPUT_FORMAT("binary")
SECTIONS {
	.text 0x1000000 : {
		code = . ; _code = . ;
		*(.text)
	}
	.rodata : {
		rodata = .;
		*(.rodata)
	}
	.rodata.str1.4 : {
		rodata.str1.4 = .;
		*(.rodata.str1.4)
	}
	.rodata.str1.1 : {
		rodata.str1.1 = .;
		*(.rodata.str1.1)
	}
	.data : {
		*(.data)
		edata = . ;
	}
	.bss : {
		bss = . ; _bss = . ;
		*(.bss)
		*(.COMMON)
	}
	end = . ; _end = . ;
}
