#include "../inc/common.h"
#include <stdint.h>
#include <stddef.h>
#include "../inc/video.h"

unsigned int current_loc = 0; // How far are we into video memory?
char *vidptr = (char*)0xb8000; // Pointer to beginning of VGA text mode memory

void kprint(const char *str) { // Basically just a wrapper function (laziness go brrrrr)
    kprintc(str, DEFAULT_COLOR);
}

void kprintc(const char *str, int color) {
	for(size_t i = 0; i < strlen(str); i++) {
		switch(str[i]) {
		case '\n':
			kprint_newline();
			break;
		case '\t':
			for(int j = 0; j < 4; j++)
				kprintc(" ", color); // Recursive functions probably aren't a great idea, but eh
			break;
		default:
			vidptr[current_loc++] = str[i];
			vidptr[current_loc++] = color;
			break;
		}
	}
}



void putc(const char str) {
	vidptr[current_loc++] = str;
	vidptr[current_loc++] = 0x07;
	
}
void kprint_newline(void)
{
	unsigned int line_size = 2 * COLUMNS;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}

void clear_screen(void) {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}
	current_loc = 0;

}