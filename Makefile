ARCH := x86_64
TARGET := $(ARCH)-elf

LD := $(TARGET)-ld
GAS := $(TARGET)-as
OBJDUMP := $(TARGET)-objdump
GRUB_MKRESCUE := grub-mkrescue
QEMU := qemu-system-$(ARCH)

LDFLAGS := -g -z max-page-size=0x1000
ASFLAGS := -g

OBJ := build
ISO_STAGING := $(OBJ)/iso
KERNEL := $(OBJ)/kernel.elf
BOOTABLE_ISO := barebones-longmode.iso

sources := $(wildcard src/**/*.s)
objects := $(addprefix $(OBJ)/, $(notdir $(sources:%.s=%.o)))
grub_config := src/boot/grub.cfg
linker_script := src/boot/linker.ld

all: qemu 

iso: $(BOOTABLE_ISO)


# assemble .s -> .o
$(OBJ)/%.o: src/**/%.s
	@mkdir -p $(OBJ)
	$(GAS) $(ASFLAGS) $^ -o $@

# link kernel
$(KERNEL): $(objects) $(linker_script)
	$(LD) $(LDFLAGS) -T $(linker_script) -o $@ $(objects)

# bootable iso 
$(BOOTABLE_ISO): $(KERNEL) $(grub_config)
	mkdir -p $(OBJ)/iso/boot/grub
	cp $(KERNEL) $(OBJ)/iso/boot/
	cp $(grub_config) $(OBJ)/iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(OBJ)/iso

qemu: $(BOOTABLE_ISO)
	$(QEMU) -cdrom $(BOOTABLE_ISO)

objdump: $(KERNEL)
	$(OBJDUMP) -hd $(KERNEL)

debug: clean $(BOOTABLE_ISO)
	$(QEMU) -s -S -cdrom $(BOOTABLE_ISO)

clean:
	rm -rf $(OBJ)
	rm -rf $(BOOTABLE_ISO)