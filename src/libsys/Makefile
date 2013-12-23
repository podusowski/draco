.SUFFIXES: .asm .c;

OUTFILE = ../../lib/libsys.o

OBJS =	system.o \
		systemcall.o

CFLAGS = -O2 -fomit-frame-pointer -I include -I ../../include -nostdinc -fno-builtin -DDEBUG

$(OUTFILE): $(OBJS)
	ld -melf_i386 -r -o $(OUTFILE) $(OBJS)

.asm.o:
	nasm $*.asm -o $*.o -f elf

.c.o:
	gcc -m32 -c $(CFLAGS) -o $*.o $*.c

