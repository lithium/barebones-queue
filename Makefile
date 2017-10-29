TARGET := i686-elf

GCC := $(TARGET)-gcc
GCCGO := $(TARGET)-gccgo
GAS := $(TARGET)-as
GRUB_MKRESCUE := grub-mkrescue

OBJ := build
ISO_STAGING := $(OBJ)/iso
KERNEL := $(OBJ)/kernel.elf
BOOTABLE_ISO := barebones-queue.iso

sources := $(wildcard src/**/*.s)
objects := $(addprefix $(OBJ)/, $(notdir $(sources:%.s=%.o)))


default: build $(BOOTABLE_ISO)

build:
	mkdir -p $(OBJ)

# assemble .s -> .o
$(OBJ)/%.o: src/**/%.s
	$(GAS) $^ -o $@

# link kernel
$(KERNEL): $(objects)
	$(GCC) -T src/boot/linker.ld -o $@ -ffreestanding -nostdlib -lgcc $^

# bootable iso 
$(BOOTABLE_ISO): $(KERNEL)
	mkdir -p $(OBJ)/iso/boot/grub
	cp $(KERNEL) $(OBJ)/iso/boot/
	cp grub/grub.cfg $(OBJ)/iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(OBJ)/iso

clean:
	rm -rf $(OBJ)