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


#########################
# Assembler & compilers #
#########################
CC       = gcc
CFLAGS   = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
           -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS  = -T $(SRC)/link.ld -melf_i386
AS       = nasm
ASFLAGS  = -f elf32

#################
# Phony targets #
#################

.PHONY: iso run clean

iso: $(OUT)/os.iso

run: $(OUT)/os.iso
	bochs -f $(ETC)/bochs.cfg -q

clean:
	rm -rf $(OUT) $(PKG)

################
# Real targets #
################

$(OUT)/os.iso: $(OUT_BIN)/kernel
	$(ETC)/restore-packages.sh
	$(ETC)/build-layout.sh
	$(ETC)/build-iso.sh

$(OUT_BIN)/kernel: $(OUT_OBJ)/boot.o $(OUT_OBJ)/kmain.o $(OUT_OBJ)/system.o $(OUT_OBJ)/monitor.o $(OUT_OBJ)/descriptor_tables.o $(OUT_OBJ)/gdt.o $(OUT_OBJ)/idt.o $(OUT_OBJ)/interrupt.o $(OUT_OBJ)/isr.o
	mkdir -p $(OUT_BIN)
	ld $(LDFLAGS) $^ -o $@

$(OUT_OBJ)/%.o: $(SRC)/%.s $(OUT)
	$(AS) $(ASFLAGS) $< -o $@

$(OUT_OBJ)/%.o: $(SRC)/%.c $(OUT)
	$(CC) $(CFLAGS) $< -o $@

$(OUT)/:
	mkdir -p $(OUT) $(OUT_OBJ) $(OUT_BIN)
