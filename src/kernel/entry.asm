[BITS 32]

[SECTION .text]

MULTIBOOT_PAGE_ALIGN   equ 1<<0
MULTIBOOT_MEMORY_INFO  equ 1<<1
MULTIBOOT_AOUT_KLUDGE  equ 1<<16
MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
CHECKSUM               equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

align 4

; wartosci z linkera
[extern code]
[extern edata] 
[extern end]

mboot:           
  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd CHECKSUM
  ;MULTIBOOT_AOUT_KLUDGE
  dd mboot ; these are PHYSICAL addresses
  dd code  ; start of kernel .text (code) section
  dd edata ; end of kernel .data section
  dd end   ; end of kernel BSS
  dd start ; kernel entry point (initial EIP)

[GLOBAL start]
start:
  ; niewiadmo co bootloader ustawi
  push dword 0x2
  popf


  ; w ebx jest wskaznik do strukture w ktorej
  ; GRUB zapisal dla nas ciekawe informacje ;)
  push ebx

  [EXTERN k_main]
  call k_main

  cli
  hlt
  
start_paging:
  mov eax, cr3
  or eax, 0x80000000
  mov cr3, eax
  ret

[GLOBAL read_cr0]
read_cr0:
  mov eax, cr0
  ret

[GLOBAL write_cr0]
write_cr0:
  push ebp
  mov ebp, esp
  mov eax, [ebp+8]
  mov cr0,  eax
  pop ebp
  ret

[GLOBAL read_cr3]
read_cr3:
  mov eax, cr3
  ret

[GLOBAL write_cr3]
write_cr3:
  push ebp
  mov ebp, esp
  mov eax, [ebp+8]
  mov cr3, eax
  pop ebp
  ret
  
[GLOBAL move_to_user_mode]  
move_to_user_mode:
  mov eax, esp
  push (8*4)|3
  push eax ;esp
  pushf
  push (8*3)|3
  push user_mode
  iret
  user_mode:
  mov ax, (8*4)|3
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  ret


[GLOBAL load_gdt]
load_gdt:
  lgdt [gdt_descr]
  ret

[GLOBAL load_idt]
load_idt:
  lidt [idt_descr]
  ret

[SECTION .data]

cpu_vendor: times 12 db 0 
            db ' detected.'

gdt_descr:
  .size: dw 2048
  .addr: dd 0x400000

idt_descr:
  .size: dw 2048
  .addr: dd 0x400400

