#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};

uint8_t vga_get_color(enum color fg, enum color bg);
void scroll(int line);
void clear();
void tputchar(char e);
void print(char *data);
void tset_color(enum color fg, enum color bg);
void tsetpos(int newx, int newy);
void tsetcur(int newx, int newy);
void tupdatecur();
size_t tgetx();
size_t tgety();
