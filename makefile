###############
# Directories #
###############
SRC      = src
OUT      = out
OUT_OBJ  = $(OUT)/obj
OUT_BIN  = $(OUT)/bin
PKG      = packages
ETC      = etc

#########
# Files #
#########
OBJFILES = $(OUT_OBJ)/boot.o \
           $(OUT_OBJ)/descriptor_tables.o \
           $(OUT_OBJ)/gdt.o \
           $(OUT_OBJ)/idt.o \
           $(OUT_OBJ)/interrupt.o \
           $(OUT_OBJ)/io.o \
           $(OUT_OBJ)/isr.o \
           $(OUT_OBJ)/kheap.o \
           $(OUT_OBJ)/kmain.o \
           $(OUT_OBJ)/monitor.o \
           $(OUT_OBJ)/ordered_array.o \
           $(OUT_OBJ)/paging.o \
           $(OUT_OBJ)/system.o \
           $(OUT_OBJ)/timer.o

#########################
# Assembler & compilers #
#########################
ifeq ($(OS),Windows_NT)
    # Windows
    $(error Windows is not a supported built platform.)
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    # Linux
    CC       = gcc
    CFLAGS   = -m32 \
               -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
               -Wall -Wextra -Werror -c
    LD       = ld
    LDFLAGS  = -T $(SRC)/link.ld -melf_i386
    AS       = nasm
    ASFLAGS  = -f elf32
    BOCHSCFG = $(ETC)/bochs.linux.cfg
  endif
  ifeq ($(UNAME_S),Darwin)
    # macOS
    CC       = clang
    CFLAGS   = -target i386-elf \
               -mno-sse \
               -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
               -Wall -Wextra -Werror -c
    LD       = $(PKG)/gnubinutils/out/bin/i386-elf-ld
    LDFLAGS  = -T $(SRC)/link.ld -melf_i386
    AS       = nasm
    ASFLAGS  = -f elf
    BOCHSCFG = $(ETC)/bochs.darwin.cfg
  endif
endif

#################
# Phony targets #
#################

.PHONY: all run iso kernel clean

all: iso

run: iso
	bochs -f $(BOCHSCFG) -q

iso: $(OUT)/os.iso

kernel: $(OUT_BIN)/kernel

clean:
	rm -rf $(OUT)

################
# Real targets #
################

$(OUT)/os.iso: $(OUT_BIN)/kernel
	$(ETC)/build-layout.sh
	$(ETC)/build-iso.sh

$(OUT_BIN)/kernel: $(OBJFILES)
	$(LD) $(LDFLAGS) $^ -o $@

$(OUT_OBJ)/%.o: $(SRC)/%.s $(OUT)
	$(AS) $(ASFLAGS) $< -o $@

$(OUT_OBJ)/%.o: $(SRC)/%.c $(OUT)
	$(CC) $(CFLAGS) $< -o $@

$(OUT):
	mkdir -p $(OUT) $(OUT_OBJ) $(OUT_BIN)
