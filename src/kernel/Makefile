.SUFFIXES: .asm .c;

OUTFILE = ../../bin/kernel

OBJS =	entry.o \
		kernel.o \
		gdt.o \
		ints.o \
		isr.o \
		paging.o \
		proc.o \
		syscall.o \
		modules.o \
		console.o \
		exception.o \
		ipc.o \
		elf.o \
		../../lib/libc.o


CFLAGS = -O2 -fomit-frame-pointer -I include -I ../../include -nostdinc -fno-builtin -DDEBUG

$(OUTFILE): $(OBJS)
	mkdir -p `dirname $@`
	ld -melf_i386 -T link.ld -o $(OUTFILE) $(OBJS)

.asm.o:
	nasm $*.asm -o $*.o -f elf

.c.o:
	gcc -m32 -c $(CFLAGS) -o $*.o $*.c

