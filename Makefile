kernel: libc
	make -C src/kernel

libc:
	make -C src/libc

crt0:
	make -C src/crt0

libsys:
	make -C src/libsys

idle: crt0
	make -C src/idle

keybd: crt0
	make -C src/keybd

sh: crt0
	make -C src/sh

storaged: crt0
	make -C src/storaged

floppyd: crt0
	make -C src/floppyd

.PHONY: clean
clean:
	rm -rf bin/
	find . -name *.o -exec rm {} \;

.PHONY: create-floppy-image
create-floppy-image:
	mkdir -p out/
	cp templates/floppy.img.template out/floppy.img
	mcopy -o -i out/floppy.img bin/ ::/
	mcopy -o -i out/floppy.img templates/MENU.LST.template ::/BOOT/GRUB/MENU.LST

.PHONY: run-bochs
run-bochs: create-floppy-image
	bochs -q -f extern/bochsrc.bxrc

.PHONY: demo
demo: clean kernel libc crt0 libsys idle keybd sh create-floppy-image run-bochs

