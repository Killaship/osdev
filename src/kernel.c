#include "../inc/video.h"
#include "../inc/system.h"
#include "../inc/time.h"


void kmain() {
    // TODO: Turn the "hello world" demo stuff into an executable to be run.
    // (it'd, of course, be much later on in the OS's development, but still)
    idt_init();
    kprint("Hello, world!\n");

    char *time = (char *)timestring(); // cast because warnings go brrr
    kprint("System Time: "); // TODO: Find a way to print leading zeroes in the time (put it in time.c)
    kprintc(time, 0x04);
    kprint_newline();
    
    char *vendorstring = (char *)cpu_string(); // another cast to get rid of annoying type warnings
    kprintc(vendorstring, 0x09);
    while(1); // there's nothing else to do, is there?
}