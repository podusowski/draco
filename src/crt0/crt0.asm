section .text
extern main
call main
xor eax, eax
int 0x80
jmp $
