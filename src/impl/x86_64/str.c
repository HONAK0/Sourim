#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size){
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}
void* memset(void* bufptr, int value, size_t size){
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}
int memcmp(const void* aptr, const void* bptr, size_t size){
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}
size_t strlen(const char *str){
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
void charadd(char arr[], char c){
	arr[strlen(arr)] = c;
}
void stradd(char arr[], char str[]){
	for(int i = 0; i < strlen(str); i++){
		charadd(arr, str[i]);
	}
}
int strcheck(char arr1[], char *arr2, int len){
	int check = 0;
	int ii = 0;
	if(strlen(arr1) > strlen(arr2)){
		return 0;
	}
	for(int i = 0; i < len; i++){
		if(arr2[ii] == arr1[i]){
			check = 1;
			ii++;
		} else if(ii == strlen(arr2)) {
			break;
		} else if(ii < strlen(arr2)) {
			check = 0;
		}
	}
	return check;
}
