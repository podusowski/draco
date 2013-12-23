NAME = keybd

OBJS = 	../../lib/libc.o \
		../../lib/libsys.o \

CFLAGS = -O2 -fomit-frame-pointer -I ../kernel/include -I ../../include -nostdinc -fno-builtin -DDEBUG

all:
	gcc -m32 -c $(CFLAGS) -o $(NAME).o $(NAME).c
	ld -melf_i386 -T link.ld -o ../../bin/$(NAME).dri ../crt0/crt0.o $(NAME).o $(OBJS)
