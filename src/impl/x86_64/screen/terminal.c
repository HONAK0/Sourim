#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "str.h"
#include "drivers/port.h"

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

void tset_color(enum color fg, enum color bg);

static inline uint8_t vga_entry_color(enum color fg, enum color bg){
	return fg | bg << 4;
}
static inline uint16_t vga_entry(unsigned char uc, uint8_t vcolor){
	return (uint16_t) uc | (uint16_t) vcolor << 8;
}
uint8_t vga_get_color(enum color fg, enum color bg){
	return vga_entry_color(fg, bg);
}

static const size_t TWIDTH = 80;
static const size_t THEIGHT = 25;
uint8_t clearcolor;
uint8_t tcolor;
uint16_t* tbuffer;
size_t pos = 0;
int x = 0;
int y = 0;
int xcur = 0;
int ycur = 0;
size_t tgetx(){
	return x;
}
size_t tgety(){
	return y;
}

void clear(){
	clearcolor = vga_entry_color(WHITE, BLACK);
	if(tcolor == 0){
		tcolor = clearcolor;
	}
    tbuffer = (uint16_t*) 0xB8000;
    for(x = 0; x < TWIDTH; x++){
        for(y = 0; y < THEIGHT; y++){
            pos = y * TWIDTH + x;
            tbuffer[pos] = vga_entry(' ', tcolor);
        }
    }
    x = 0;
    y = 0;
}
void tupdatecur(){
	uint16_t pos = ycur * TWIDTH + xcur;
	port_word_out(0x3D4, 0x0F);
	port_word_out(0x3D5, (uint8_t) (pos & 0xFF));
	port_word_out(0x3D4, 0x0E);
	port_word_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
void tsetcur(int newx, int newy){
	xcur = newx;
	ycur = newy;
}
void scroll() {
	tset_color(WHITE, BLACK);
	clear();
}
void tputchar(char e){
	switch(e){
		case '\n':
			y++;
			x=0;
			break;
		case '\r':
			x=0;
			break;
		case '\1':
			if(x == 0){
                x = TWIDTH+1;
                y--;
            }
			x--;
            pos = y * TWIDTH + x;
			tbuffer[pos] = vga_entry(' ', tcolor);
			break;
		case '\2':
			if(y == 0){
				y = 1;
			}
			y--;
			break;
		default:
			pos = y * TWIDTH + x;
            tbuffer[pos] = vga_entry(e, tcolor);
			if(x == TWIDTH){
				x=0;
				y++;
			}
			x++;
			break;
	}
	tsetcur(x, y);
    if(xcur > 80){
        xcur = 0;
        ycur++;
    }
	if(y >= THEIGHT){
		y--;
		scroll();
	}
	tupdatecur();
}
void tsetpos(int newx, int newy){
	x = newx;
	y = newy;
}

void print(char *data){
    for(size_t index = 0; index < strlen(data); index++){
        tputchar(data[index]);
    }
}

void tset_color(enum color fg, enum color bg){
    tcolor = vga_entry_color(fg, bg);
}
