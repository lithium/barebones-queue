TARGET := i686-elf

GCC := $(TARGET)-gcc
GCCGO := $(TARGET)-gccgo
GAS := $(TARGET)-as
OBJDUMP := $(TARGET)-objdump
GRUB_MKRESCUE := grub-mkrescue
QEMU := qemu-system-i386

OBJ := build
ISO_STAGING := $(OBJ)/iso
KERNEL := $(OBJ)/kernel.elf
BOOTABLE_ISO := barebones-queue.iso

sources := $(wildcard src/**/*.s)
objects := $(addprefix $(OBJ)/, $(notdir $(sources:%.s=%.o)))


all: build qemu


build:
	mkdir -p $(OBJ)

# assemble .s -> .o
$(OBJ)/%.o: src/**/%.s
	$(GAS) -g $^ -o $@

# link kernel
$(KERNEL): $(objects)
	$(GCC) -g -T src/boot/linker.ld -o $@ -ffreestanding -nostdlib -lgcc $^

# bootable iso 
$(BOOTABLE_ISO): $(KERNEL)
	mkdir -p $(OBJ)/iso/boot/grub
	cp $(KERNEL) $(OBJ)/iso/boot/
	cp grub/grub.cfg $(OBJ)/iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(OBJ)/iso

qemu: $(KERNEL)
	$(QEMU) -kernel $(KERNEL)

debug: $(KERNEL)
	$(OBJDUMP) -d $(KERNEL)
	$(QEMU) -s -S -kernel $(KERNEL)

clean:
	rm -rf $(OBJ)