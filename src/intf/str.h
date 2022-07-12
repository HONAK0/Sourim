#include <stddef.h>

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void* bufptr, int value, size_t size);
size_t strlen(const char str[]);
void stradd(char arr[], char str[]);
void charadd(char arr[], char c);
int strcheck(char arr1[], char *arr2, int len);
