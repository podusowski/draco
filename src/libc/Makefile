.SUFFIXES: .asm .c;

OUTFILE = ../../lib/libc.o

OBJS =	memcpy.o \
		memset.o \
		sprintf.o \
		printf.o \
		strlen.o \
		ultoa.o \
		strchr.o \
		strcmp.o


CFLAGS = -O2 -fomit-frame-pointer -I include -I ../../include -nostdinc -fno-builtin -DDEBUG

$(OUTFILE): $(OBJS)
	mkdir -p `dirname $@`
	ld -melf_i386 -r -o $(OUTFILE) $(OBJS)

.asm.o:
	nasm $*.asm -o $*.o -f elf

.c.o:
	gcc -m32 -c $(CFLAGS) -o $*.o $*.c

