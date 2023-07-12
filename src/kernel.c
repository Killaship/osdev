#include "../inc/video.h"
#include "../inc/system.h"
#include "../inc/time.h"
#include "../inc/common.h"

void kmain() {
    idt_init();
    kprint("Hello, world!\n");

    kprint("System Time: "); // TODO: Find a way to print leading zeroes in the time
	kprint(itoa(gettime(2))); // hours
	kprint(":");
	kprint(itoa(gettime(1))); // minutes
	kprint(":");
	kprint(itoa(gettime(0))); // seconds

    while(1);
}