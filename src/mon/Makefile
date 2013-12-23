NAME = mon

OBJS = 	../../lib/libc.o \
		../../lib/libsys.o \

CFLAGS = -O2 -fomit-frame-pointer -I ../kernel/include -I ../../include -nostdinc -fno-builtin -DDEBUG

all:
	/usr/cross/i586-elf/bin/gcc -c $(CFLAGS) -o $(NAME).o $(NAME).c
	ld -T /usr/cross/i586-elf/lib/ldscripts/elf_i386.x -o ../../bin/$(NAME).dri ../crt0/crt0.o $(NAME).o $(OBJS)
