ENTR = entry.o
OBJS = malloc.o stdio.o string.o printf.o
ASMS = entry.asm \
		malloc.asm stdio.asm string.asm printf.asm

CC = gcc
CFLAGS = -fno-builtin -nostdlib -fno-stack-protector
CFLAGS += -m32 -Wall -Werror
LD = ld
LDFLAGS = -static -e mini_crt_entry
OBJDUMP = objdump

all: archive asm

archive: $(OBJS)
	ar -rs minicrt.a $(OBJS) 

asm: $(ASMS)

%.asm: %.o
	$(OBJDUMP) -S $< > $*.asm

# not working yet
# how to compile and link 32-bit program in 64-bit machine
test: $(ENTR) archive
	$(CC) $(CFLAGS) -ggdb -o test.o test.c
	$(LD) $(LDFLAGS) $(ENTR) test.o minicrt.a -o $@

.PHONY: clean

clean:
	rm -f *.o *.a *.asm
