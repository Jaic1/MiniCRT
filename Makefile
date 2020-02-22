ENTR = entry.o
OBJS = malloc.o stdio.o string.o printf.o
ASMS = entry.asm \
		malloc.asm stdio.asm string.asm printf.asm

CC = gcc
CFLAGS = -fno-builtin -nostdlib -fno-stack-protector
CFLAGS += -mx32 -Wall -Werror -ggdb
LD = ld
LDFLAGS = -static -e mini_crt_entry -m elf32_x86_64
OBJDUMP = objdump

all: archive asm

archive: $(OBJS)
	ar -rs minicrt.a $(OBJS) 

asm: $(ASMS)

%.asm: %.o
	$(OBJDUMP) -S $< > $*.asm

test: $(ENTR) archive test.o
	$(LD) $(LDFLAGS) $(ENTR) test.o minicrt.a -o $@
	$(OBJDUMP) -S $@ > $@.asm

.PHONY: clean

clean:
	rm -f *.o *.a *.asm test *.txt
