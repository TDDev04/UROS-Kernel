
CFLAGS=-std=gnu99 -ffreestanding -Os -fpeel-loops -floop-interchange -floop-unroll-and-jam -fpredictive-commoning -ftree-loop-distribution -ftree-partial-pre -funswitch-loops -Isrc\include
OBJ=obj/misc.o obj/ldr_idt.o obj/kc.o obj/ports.o obj/idt.o
CC=gcc -m32
AS=nasm
LD=ld -m elf_i386


KERNEL := kernel.sys
OBJ := obj
CFILES := $(shell find ./src/ -type f -name '*.c' -not -path "initramfs/*")
ASMFILES = $(shell find ./src/ -type f -name '*.asm' -not -path "initramfs/*")
OBJ = $(CFILES:.c=.o)
OBJ += $(ASMFILES:.asm=_asm.o)


all: $(KERNEL)

$(KERNEL): $(OBJ)
	$(LD) -T ./bootloader/linker.ld -o src/$(KERNEL) $(OBJ)

	$(AS) -fbin bootloader/settings.asm -o obj/settings.bin
	$(AS) -fbin bootloader/bootloader.asm -o image/UROS-IMAGE.bin

%.o: %.c
	$(CC) $(CFLAGS) -fno-stack-protector -c $< -o $@

%_asm.o: %.asm
	$(AS) $^ -f elf32 -o $@

clean:
	rm -rf $(OBJ)