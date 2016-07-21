###############
# Directories #
###############
SRC     = src
OUT     = out
OUT_OBJ = $(OUT)/obj
OUT_BIN = $(OUT)/bin
PKG     = packages
ETC     = etc

#########
# Files #
#########


#########################
# Assembler & compilers #
#########################
CC      = gcc
CFLAGS  = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
          -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T $(SRC)/boot/link.ld -melf_i386
AS      = nasm
ASFLAGS = -f elf

#################
# Phony targets #
#################

.PHONY: iso run outdir clean

iso: $(OUT)/os.iso

run: $(OUT)/os.iso
	bochs -f $(ETC)/bochs.cfg -q

clean:
	rm -rf $(OUT) $(PKG)

################
# Real targets #
################

$(OUT)/os.iso: $(OUT_BIN)/kernel.elf
	$(ETC)/restore-packages.sh
	$(ETC)/build-layout.sh
	$(ETC)/build-iso.sh

$(OUT_BIN)/kernel.elf: $(OUT_OBJ)/loader.o
	mkdir -p $(OUT_BIN)
	ld $(LDFLAGS) $^ -o $@

$(OUT_OBJ)/loader.o: $(SRC)/boot/loader.s
	mkdir -p $(OUT_OBJ)
	$(AS) $(ASFLAGS) $< -o $@

