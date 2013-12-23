NAME = test

OBJS = 	../../libc/memcpy.o \
		../../libc/memset.o \
		../../libc/sprintf.o \
		../../libc/printf.o \
		../../libc/strlen.o \
		../../libc/ultoa.o \
		../../libc/strchr.o \
		../../libc/strcmp.o \
		../../libc/system.o \
		../../libc/systemcall.o

CFLAGS = -O2 -fomit-frame-pointer -I ../../kernel/include -I ../../libc/include -nostdinc -fno-builtin -DDEBUG

all:
	gcc -c $(CFLAGS) -o $(NAME).o $(NAME).c
	#gcc -S $(CFLAGS) -o $(NAME).s $(NAME).c
	ld -Map map.txt -T link.ld -o ../../bin/$(NAME).dri --start-group ../entry/entry.o $(NAME).o $(OBJS) --end-group
