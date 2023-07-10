#define DEFAULT_COLOR 0x07 // Color used for kprint(). kprintc() doesn't use this.

// Some defines to calculate screen size related-things
#define LINES 25 
#define COLUMNS 80
#define SCREENSIZE 2 * COLUMNS * LINES


void kprintc(const char *str, int color);
void kprint(const char *str);
void putc(const char str);
void kprint_newline(void);
void clear_screen(void);