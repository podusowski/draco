[BITS 32]

[SECTION .text]

extern kernel_esp
extern tss
extern user_ring0_esp
extern timer_handler

global timer_isr
timer_isr:
	;tu mamy esp z tssa
	cli

	pushad
	push ds
	push es
	push fs
	push gs

	mov ax, 16
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov [user_ring0_esp], esp
	mov esp, [kernel_esp]

	call timer_handler
	;nie czyscimy stosu bo i tak jest w kernel_esp

	global timer_isr_part2
	timer_isr_part2:

	mov esp, [user_ring0_esp]

	mov al , 0x20
	out 0x20 , al

	pop gs
	pop fs
	pop es
	pop ds
	popad

	;czyli esp nie liczac zapisanego stanu
	mov [tss+4], esp
	add long [tss+4], 20

	sti
	iret

[GLOBAL int80_isr]
int80_isr:
	cli

	;tu mamy esp z tssa

	pushad
	push ds
	push es
	push fs
	push gs

	push ax
	mov ax, 16
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	pop ax

	;potrzebny!
	mov [user_ring0_esp], esp
	mov esp, [kernel_esp]

	push edx
	push ecx
	push ebx
	push eax

	extern do_syscall
	call do_syscall

	add esp, 16

	mov esp, [user_ring0_esp]

	pop gs
	pop fs
	pop es
	pop ds
	
	add esp, 32
	;popad

	mov [tss+4], esp
	add long [tss+4], 20

	sti
	iret

extern irq_handler

%macro IRQ_ISR 1
global irq_isr%1
irq_isr%1:
	cli

	pusha
	push gs
	push fs
	push es
	push ds

	push ax
	mov ax, 16
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	pop ax

	;uzywamy stosu aplikacji/kernela

	push %1
	call irq_handler
	add esp, 4
	
	pop ds
	pop es
	pop fs
	pop gs
	popa

	sti
	iret
%endmacro

IRQ_ISR 1
IRQ_ISR 2
IRQ_ISR 3
IRQ_ISR 4
IRQ_ISR 5
IRQ_ISR 6
IRQ_ISR 7
IRQ_ISR 8
IRQ_ISR 9
IRQ_ISR 10
IRQ_ISR 11
IRQ_ISR 12
IRQ_ISR 13
IRQ_ISR 14
IRQ_ISR 15
IRQ_ISR 16

extern exception_hardware 
 
%macro EXCEPTION_ISR 1
[GLOBAL exception_isr%1]
exception_isr%1:
	cli

	pusha
	push gs
	push fs
	push es
	push ds

	mov eax, cr2
	push eax
	push %1
	call exception_hardware
	add esp, 8
	
	pop ds
	pop es
	pop fs
	pop gs
	popa

	sti

	iret
%endmacro

EXCEPTION_ISR 0
EXCEPTION_ISR 1
EXCEPTION_ISR 2
EXCEPTION_ISR 3
EXCEPTION_ISR 4
EXCEPTION_ISR 5
EXCEPTION_ISR 6
EXCEPTION_ISR 7
EXCEPTION_ISR 8
EXCEPTION_ISR 9
EXCEPTION_ISR 10
EXCEPTION_ISR 11
EXCEPTION_ISR 12
EXCEPTION_ISR 13
EXCEPTION_ISR 14
EXCEPTION_ISR 15
EXCEPTION_ISR 16


[GLOBAL unknown_interrupt_isr]
unknown_interrupt_isr:
	iret
