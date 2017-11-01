
barebones 64-bit longmode bootstrap in gnuas (at&t)
multiboot2


## Toolchain

### binutils 

gnu as, ld, objdump/objcopy

depends on: gmp, mpfr, libmpc (all in homebrew)

	https://ftp.gnu.org/gnu/binutils/

	export TARGET=x86_64-elf
	export PREFIX=/opt/$TARGET-toolchain

	../binutils-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
	make && make install


### grub2

depends on: objconv, automake


### gdb

	https://ftp.gnu.org/gnu/gdb/

	export TARGET=x86_64-elf
	export PREFIX=/opt/$TARGET-toolchain
	../gdb-x.y.z/configure --target=$TARGET --prefix="$PREFIX"
	make && make install


patch according to Workaround 2 in this thread: http://wiki.osdev.org/QEMU_and_GDB_in_long_mode
in ./gdb/remote.c, find the "packet reply is too long", remove the error line and replace with the block in the patch.  

This means gdb can load elf64 symbol tables, and can remote debug qemu in real, protected and long mode. (though gdb register information is only 32 bits)



### qemu

available in homebrew




## References

http://wiki.osdev.org/GCC_Cross-Compiler
https://gist.github.com/emkay/a1214c753e8c975d95b4

http://wiki.osdev.org/Setting_Up_Long_Mode
http://wiki.osdev.org/QEMU_and_GDB_in_long_mode
