ENTR = entry.o
COBJS = malloc.o stdio.o string.o printf.o atexit.o
CXXOBJS = crtbegin.o crtend.o ctors.o new_delete.o sysdep.o iostream.o
ASMS = entry.asm malloc.asm stdio.asm string.asm printf.asm atexit.asm \
	crtbegin.asm crtend.asm ctors.asm new_delete.asm sysdep.asm iostream.asm

CC = gcc
CFLAGS = -fno-builtin -nostdlib -fno-stack-protector
CFLAGS += -mx32 -Wall -Werror -ggdb
CXX = g++
CXXFLAGS = -nostdinc++ -fno-rtti -fno-exceptions -fno-builtin -nostdlib -fno-stack-protector
CXXFLAGS += -mx32 -Wall -Werror -ggdb
LD = ld
LDFLAGS = -static -e mini_crt_entry -m elf32_x86_64
OBJDUMP = objdump

all: archive asm

archive: $(COBJS) $(CXXOBJS)
	ar -rs minicrt.a \
	malloc.o stdio.o string.o printf.o atexit.o \
	ctors.o iostream.o new_delete.o sysdep.o

asm: $(ASMS)

%.asm: %.o
	$(OBJDUMP) -S $< > $*.asm

testc: $(ENTR) archive testc.o
	$(LD) $(LDFLAGS) $(ENTR) crtbegin.o testc.o minicrt.a crtend.o \
		-o $@
	$(OBJDUMP) -S $@ > $@.asm

testcpp: $(ENTR) archive testcpp.o
	$(LD) $(LDFLAGS) $(ENTR) crtbegin.o testcpp.o minicrt.a crtend.o \
		-o $@
	$(OBJDUMP) -S $@ > $@.asm

.PHONY: clean

clean:
	rm -f *.o *.a *.asm *.txt testc testcpp
	rm -rf .vscode
