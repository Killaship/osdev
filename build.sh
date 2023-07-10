# Treat warnings as errors, disable stack protector, 
# compile for 32-bit, enable debug symbols, compile/assemble but don't link, compile for freestanding
gcc -Werror -fno-stack-protector -m32 -g -c src/kernel.c -o kernel.o -ffreestanding
gcc -Werror -fno-stack-protector -m32 -g -c src/video.c -o video.o -ffreestanding
gcc -Werror -fno-stack-protector -m32 -g -c src/common.c -o common.o -ffreestanding



# Compile as x86 ELF, spits out an object file
nasm -f elf32 src/kernel.asm -o kasm.o

# Link in ELF (i386) format, use our custom linker script, use debug symbols,
# have our output be kernel.elf, and the rest are the object files to be linked
ld -m elf_i386 -T misc/link.ld -g -o kernel.elf kasm.o kernel.o video.o common.o

# Generate the directories used for generating the ISO, creating parent directories if needed
mkdir -p iso/boot/grub
mkdir -p iso/modules

# Copy kernel binary & GRUB config to the disc directory
cp kernel.elf iso/boot/kernel.elf
cp misc/grub.cfg iso/boot/grub/grub.cfg

# Generate the ISO file
grub-mkrescue -o bin/os.iso iso

# Clean up
rm *.o
rm *.elf
rm -rf iso

# Run QEMU as if the OS is run off a CD, and set the RTC to the same time as the host computer
qemu-system-i386 -cdrom bin/os.iso -rtc base=localtime 
