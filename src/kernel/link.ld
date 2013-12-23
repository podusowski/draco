OUTPUT_FORMAT("binary")
ENTRY("k_main")
SECTIONS {
    .text 0x100000 : {
        code = . ; _code = . ;
        *(.text)
    }
    .data : {
        *(.data)
    }
    edata = .;
    .bss : {
        bss = . ; _bss = . ;
        *(.bss)
        *(.COMMON)
    }
    end = . ; _end = . ;
} 
