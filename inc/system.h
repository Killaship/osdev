#include <stdint.h>
#include <stddef.h>

#define BSOD_COLOR 0xF1

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define TRAP_GATE 0b10001111
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

char read_port(unsigned short port);
void write_port(unsigned short port, unsigned char data);

void cli(void);
void sti(void);

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

const char *cpu_string();
int cpuid_string(int code, int where[4]);
void reboot(); // todo: find out alternate/better ways to reboot (ACPI?)

void idt_init(void);
void panic(void);
void install_idt(int IRQ, void handler());