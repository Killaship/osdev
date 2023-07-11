#include "../inc/video.h"
#include "../inc/system.h"

void kmain() {
    idt_init();
    kprint("Hello, world!");
    while(1);
}