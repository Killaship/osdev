// This file contains things like exception handling, and setting up the IDT, as well as port I/O,
// and miscellaneous CPU-related functions.
#include "../inc/system.h"
#include "../inc/video.h"
#include "../inc/common.h"

typedef void (*f)(void);
f idtfunc[32];
struct IDT_entry IDT[IDT_SIZE];


char read_port(unsigned short port) {
	unsigned char value;
	asm volatile ("inb %%dx, %%al" : "=a" (value) : "d" (port));
	return value;
}

void cli() {asm("cli");} // Disable interrupts
void sti() {asm("sti");} // Restore interrupts


void write_port(unsigned short port, unsigned char data) {
	asm volatile ("outb %%al, %%dx" :: "a" (data), "d" (port));
}

static inline int cpuid_string(int code, int where[4]) {
  __asm__ volatile ("cpuid":"=a"(*where),"=b"(*(where+0)),
               "=d"(*(where+1)),"=c"(*(where+2)):"a"(code));
  return (int)where[0];
}
 
const char *cpu_string() {
	static char s[16] = "BogusProces!";
	cpuid_string(0, (int*)(s));
	return s;
}


void reboot() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = read_port(0x64);
    write_port(0x64, 0xFE);
}


void load_idt(unsigned long *idt_ptr) {
	asm volatile ("lidt (%%eax)" :: "a" (idt_ptr));
}

void unmaskIRQ(int irq) {
       static unsigned short int ocw1 = 0xffff;
        ocw1 &= ~(1 << irq);     // enable propriate bit with shifting to left
                                 // invert to enable the interrupt
                                 // use & to not disturb other bits
        if (irq < 8)
                write_port(0x21, ocw1 & 0xFF);  // AND with 0xFF to clear the high 8
                                                 //  bits because we send to PIC1
        else
                write_port(0xA1, ocw1 >> 8);     // move high 8 bits to low 8 bits
                                                // since we send to PIC2
}

void idt_init(void) {
	
 	unsigned long addr_address[32] = {0};
	unsigned long idt_address;
	unsigned long idt_ptr[2];
	
	int i = 0;
	while(i < 31) {
			addr_address[i] = (unsigned long)idtfunc[i];
			IDT[i].offset_lowerbits = addr_address[i] & 0xffff;
			IDT[i].selector = KERNEL_CODE_SEGMENT_OFFSET;
			IDT[i].zero = 0;
			IDT[i].type_attr = INTERRUPT_GATE;
			IDT[i].offset_higherbits = (addr_address[i] & 0xffff0000) >> 16;	
		i++;
	}

	write_port(0x20, 0x11); // TODO: split this stuff off into a small function
	write_port(0xA0, 0x11);
	write_port(0x21, 0x20);
	write_port(0xA1, 0x28);
	write_port(0x21, 0x04);
	write_port(0xA1, 0x02);
	write_port(0x21, 0x01);
	write_port(0xA1, 0x01);
	write_port(0x21, 0xff);
	write_port(0xA1, 0xff);



	idt_address = (unsigned long)IDT;
	idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
	
}


struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};
void stacktrace(unsigned int maxframes) { // Stack tracer
	char addr[16];
	struct stackframe *stk;
	asm volatile("movl %%ebp,%0" : "=r"(stk) ::);
	kprintc("Stack trace:\n", BSOD_COLOR);
	for(unsigned int frame = 0; stk && frame < maxframes; ++frame) {
        	// Unwind to previous stack frame
		prntnum(stk->eip,16,' ',addr);
		kprintc("0x", BSOD_COLOR);
        	kprintc(addr, BSOD_COLOR);
		kprint_newline();
        	stk = stk->ebp;
	}
}

void bsod(const int stopcode) {
	paint_screen(BSOD_COLOR);
    kprintc("The operating system encountered an problem, and stopped to avoid (more) errors.", BSOD_COLOR);
    kprintc("Below is some debug information. Please record it and report the problem.\n", BSOD_COLOR);
	switch(stopcode) {
		case 0: // #DE
			kprintc("Error type: #DE", BSOD_COLOR);
			break;
		case 1: // #DB
			kprintc("Error type: #DB", BSOD_COLOR);
			break;
		case 2: // NMI
			kprintc("Error type: NMI", BSOD_COLOR);
			break;
		case 3: // #BP
			kprintc("Error type: #BP", BSOD_COLOR);
			break;
		case 4: // #OF
			kprintc("Error type: #OF", BSOD_COLOR);
			break;
		case 5: // #BR
			kprintc("Error type: #BR", BSOD_COLOR);
			break;
		case 6: // #UD
			kprintc("Error type: #UD", BSOD_COLOR);
			break;
		case 7: // #NM
			kprintc("Error type: #NM", BSOD_COLOR);
			break;
		case 8: // #DF
			kprintc("Error type: #DF", BSOD_COLOR);
			break;
		case 9: // CSO
			kprintc("Error type: CSO", BSOD_COLOR);
			break;
		case 10: // #TS
			kprintc("Error type: #TS", BSOD_COLOR);
			break;
		case 11: // #NP
			kprintc("Error type: #NP", BSOD_COLOR);
			break;
		case 12: // #SS
			kprintc("Error type: #SS", BSOD_COLOR);
			break;
		case 13: // #GP
			kprintc("Error type: #GP", BSOD_COLOR);
			break;
		case 14: // #PF
			kprintc("Error type: #PF", BSOD_COLOR);
			break;
		case 15: // Intel Reserved
			kprintc("Error type: INTELRSVD", BSOD_COLOR);
			break;
		case 16: // #MF
			kprintc("Error type: #MF", BSOD_COLOR);
			break;
		case 17: // #AC
			kprintc("Error type: #AC", BSOD_COLOR);
			break;
		case 18: // #MC
			kprintc("Error type: #MC", BSOD_COLOR);
			break;
		case 19: // #XF
			kprintc("Error type: #XF", BSOD_COLOR);
			break;
		case 20: // #VE
			kprintc("Error type: #VE", BSOD_COLOR);
			break;
		case 21: // #CP
			kprintc("Error type: #CP", BSOD_COLOR);
			break;
		case 22: // #HV
			kprintc("Error type: #HV", BSOD_COLOR);
			break;
		case 23: // #VC
			kprintc("Error type: #VC", BSOD_COLOR);
			break;
		case 24: // #SX
			kprintc("Error type: #SX", BSOD_COLOR);
			break;
	}
	kprint_newline();
	stacktrace(12);
	
	
}

// TODO: fix this ungodly mess

void err_handler() {
	// Say there's been an error, and then hang.
	bsod(0);
	asm volatile ("cli");
	while(1);
}

void err1_handler() {
	// Say there's been an error, and then hang.
	bsod(1);
	asm volatile ("cli");
	while(1);
}

void err2_handler() {
	// Say there's been an error, and then hang.
	bsod(2);
	asm volatile ("cli");
	while(1);
}

void err3_handler() {
	// Say there's been an error, and then hang.
	bsod(3);
	asm volatile ("cli");
	while(1);
}

void err4_handler() {
	// Say there's been an error, and then hang.
	bsod(4);
	asm volatile ("cli");
	while(1);
}

void err5_handler() {
	// Say there's been an error, and then hang.
	bsod(5);
	asm volatile ("cli");
	while(1);
}

void err6_handler() {
	// Say there's been an error, and then hang.
	bsod(6);
	asm volatile ("cli");
	while(1);
}

void err7_handler() {
	// Say there's been an error, and then hang.
	bsod(7);
	asm volatile ("cli");
	while(1);
}

void err8_handler() {
	// Say there's been an error, and then hang.
	bsod(8);
	asm volatile ("cli");
	while(1);
}

void err9_handler() {
	// Say there's been an error, and then hang.
	bsod(9);
	asm volatile ("cli");
	while(1);
}

void err10_handler() {
	// Say there's been an error, and then hang.
	bsod(10);
	asm volatile ("cli");
	while(1);
}

void err11_handler() {
	// Say there's been an error, and then hang.
	bsod(11);
	asm volatile ("cli");
	while(1);
}

void err12_handler() {
	// Say there's been an error, and then hang.
	bsod(12);
	asm volatile ("cli");
	while(1);
}

void err13_handler() {
	// Say there's been an error, and then hang.
	bsod(13);
	asm volatile ("cli");
	while(1);
}

void err14_handler() {
	// Say there's been an error, and then hang.
	bsod(14);
	asm volatile ("cli");
	while(1);
}

void err15_handler() {
	// Say there's been an error, and then hang.
	bsod(15);
	asm volatile ("cli");
	while(1);
}

void err16_handler() {
	// Say there's been an error, and then hang.
	bsod(16);
	asm volatile ("cli");
	while(1);
}

void err17_handler() {
	// Say there's been an error, and then hang.
	bsod(17);
	asm volatile ("cli");
	while(1);
}

void err18_handler() {
	// Say there's been an error, and then hang.
	bsod(18);
	asm volatile ("cli");
	while(1);
}

void err19_handler() {
	// Say there's been an error, and then hang.
	bsod(19);
	asm volatile ("cli");
	while(1);
}

void err20_handler() {
	// Say there's been an error, and then hang.
	bsod(20);
	asm volatile ("cli");
	while(1);
}

void err21_handler() {
	// Say there's been an error, and then hang.
	bsod(21);
	asm volatile ("cli");
	while(1);
}
void err22_handler() {
	// Say there's been an error, and then hang.
	bsod(22);
	asm volatile ("cli");
	while(1);
}
void err23_handler() {
	// Say there's been an error, and then hang.
	bsod(23);
	asm volatile ("cli");
	while(1);
}
void err24_handler() {
	// Say there's been an error, and then hang.
	bsod(24);
	asm volatile ("cli");
	while(1);
}


f idtfunc[32] = {&err_handler, &err1_handler, &err2_handler, &err3_handler, &err4_handler, &err5_handler, &err6_handler, &err7_handler,&err8_handler,&err9_handler,&err10_handler,&err11_handler,&err12_handler,&err13_handler,&err14_handler,&err15_handler,&err16_handler,&err17_handler,&err18_handler,&err19_handler,&err20_handler,&err21_handler,&err15_handler,&err15_handler,&err15_handler,&err15_handler,&err22_handler,&err23_handler,&err24_handler,&err15_handler};