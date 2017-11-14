ARCH := x86_64
TARGET := $(ARCH)-elf

LD := $(TARGET)-ld
GAS := $(TARGET)-as
CC := clang
OBJDUMP := $(TARGET)-objdump
GRUB_MKRESCUE := grub-mkrescue
QEMU := qemu-system-$(ARCH) -cpu core2duo -smp sockets=1,cores=2,threads=1

CFLAGS := -g -target $(TARGET) -I src
LDFLAGS := -g -z max-page-size=0x1000
ASFLAGS := -g

OBJ := build
ISO_STAGING := $(OBJ)/iso
KERNEL := $(OBJ)/kernel.elf
BOOTABLE_ISO := barebones-longmode.iso

assembler_sources := $(shell find src/ -type f -name '*.s')
assembler_objects := $(addprefix $(OBJ)/, $(notdir $(assembler_sources:%.s=%.o)))

# c_sources := $(wildcard src/**/*.c)
c_sources := $(shell find src/ -type f -name '*.c')
c_objects := $(addprefix $(OBJ)/, $(notdir $(c_sources:%.c=%.o)))

grub_config := src/boot/grub.cfg
linker_script := src/boot/linker.ld

all: qemu 

iso: $(BOOTABLE_ISO)


# assemble .s -> .o
$(OBJ)/%.o: src/**/%.s
	@mkdir -p $(OBJ)
	$(GAS) $(ASFLAGS) $^ -o $@
	
$(OBJ)/%.o: src/**/**/%.s
	@mkdir -p $(OBJ)
	$(GAS) $(ASFLAGS) $^ -o $@

# assemble .c -> .o
$(OBJ)/%.o: src/**/%.c
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ)/%.o: src/**/**/%.c
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $^ -o $@

# link kernel
$(KERNEL): $(assembler_objects) $(c_objects) $(linker_script)
	$(LD) $(LDFLAGS) -T $(linker_script) -o $@ $(assembler_objects) $(c_objects)

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