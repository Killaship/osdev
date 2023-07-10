#include <stdint.h>
#include <stddef.h>

#define sizeof(type) (char *)(&type+1)-(char*)(&type)
void* memset(void* bufptr, int value, size_t size);
char *strncpy(char *s1, const char *s2, size_t n);
void* memcpy(void* dstptr, const void* srcptr, size_t size);
int strncmp(const char *s1, const char *s2, size_t n);
int memcmp(const void* aptr, const void* bptr, size_t size);
size_t strlen(const char* str);
void* memmove(void* dstptr, const void* srcptr, size_t size);
char* itoa(int i);
int strcmp(const char *s1, const char *s2);
int oct2bin(unsigned char *str, int size);
void prntnum(unsigned long num, int base, char sign, char *outbuf);