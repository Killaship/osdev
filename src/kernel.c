#include "../inc/video.h"
#include "../inc/system.h"
#include "../inc/time.h"
#include "../inc/common.h"

void kmain() {
    idt_init();
    kprint("Hello, world!\n");

    char *time = (char *)timestring(); // cast because warnings go brrr
    kprint("System Time: "); // TODO: Find a way to print leading zeroes in the time
    kprintc(time,0x04);
    kprint_newline();
    
    char *vendorstring = (char *)cpu_string(); // another cast to get rid of annoying type warnings
    kprintc(vendorstring , 0x09);

    while(1);
}