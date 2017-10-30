TARGET := x86_64-elf

LD := $(TARGET)-ld
GAS := $(TARGET)-as
OBJDUMP := $(TARGET)-objdump
GRUB_MKRESCUE := grub-mkrescue
QEMU := qemu-system-x86_64

LDFLAGS := -g -z max-page-size=0x1000
ASFLAGS := -g

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
	$(GAS) $(ASFLAGS) $^ -o $@

# link kernel
$(KERNEL): $(objects)
	$(LD) $(LDFLAGS) -T src/boot/linker.ld -o $@ $^

# bootable iso 
$(BOOTABLE_ISO): build $(KERNEL)
	mkdir -p $(OBJ)/iso/boot/grub
	cp $(KERNEL) $(OBJ)/iso/boot/
	cp grub/grub.cfg $(OBJ)/iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(OBJ)/iso

qemu: build $(KERNEL)
	$(QEMU) -kernel $(KERNEL)

cdrom: $(BOOTABLE_ISO)
	$(QEMU) -cdrom $(BOOTABLE_ISO)

debug: clean build $(KERNEL)
	$(OBJDUMP) -d $(KERNEL)
	qemu-system-i386 -s -S -cdrom $(BOOTABLE_ISO)

clean:
	rm -rf $(OBJ)