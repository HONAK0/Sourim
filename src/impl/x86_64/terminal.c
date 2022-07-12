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

static inline uint8_t vga_entry_color(enum color fg, enum color bg)
{
	return fg | bg << 4;
}
static inline uint16_t vga_entry(unsigned char uc, uint8_t vcolor)
{
	return (uint16_t) uc | (uint16_t) vcolor << 8;
}
uint8_t vga_get_color(enum color fg, enum color bg){
	return vga_entry_color(fg, bg);
}

static const size_t TWIDTH = 80;
static const size_t THEIGHT = 25;
 
size_t trow;
size_t tcolumn;
uint8_t clearcolor;
uint8_t tcolor;
uint16_t* tbuffer;
size_t i = 0;
size_t x = -1;
size_t y = 0;
int xcur;
int ycur;
size_t tgetx(){
	return x;
}
size_t tgety(){
	return y;
}

void clear(){
    trow = 0;
    tcolumn = 0;
	clearcolor = vga_entry_color(WHITE, BLACK);
	if(tcolor == 0){
		tcolor = clearcolor;
	}
    tbuffer = (uint16_t*) 0xB8000;
    for(x = 0; x < TWIDTH; x++){
        for(y = 0; y < THEIGHT; y++){
            i = y * TWIDTH + x;
            tbuffer[i] = vga_entry(' ', tcolor);
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
	clear();
	/*for(int loop = 0; loop < TWIDTH; loop++) {
		i = 0 * TWIDTH + loop;
		tbuffer[i] = vga_entry(' ', tcolor);
	}
	for(int loopx = 0; loopx < TWIDTH; loopx++){
		for(int loopy = 1; loopy < THEIGHT; loopy++){
			i = loopy * TWIDTH + loopx;
			uint16_t symbol = tbuffer[i];
			tbuffer[i] = vga_entry(' ', tcolor);
			i = loopy-1 * TWIDTH + loopx;
			tbuffer[i] = symbol;
		}
	}*/
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
			x--;
            i = y * TWIDTH + x;
			tbuffer[i] = vga_entry(' ', tcolor);
            if(x < 0){
                x = TWIDTH;
                y--;
            }
			break;
		default:
			i = y * TWIDTH + x;
            tbuffer[i] = vga_entry(e, tcolor);
			if(x+1 == TWIDTH){
				x=0;
				y++;
			} else{
                x++;
            }
			if(y >= THEIGHT){
				y--;
				scroll();
			}
			break;
	}
	tsetcur(x, y);
    if(xcur > 80){
        xcur = 0;
        ycur++;
    }
	tupdatecur();
}
void tsetpos(int newx, int newy){
	x = newx;
	y = newy;
}

void print(char *data){
    size_t index = 0;
    for(; index < strlen(data); index++){
        tputchar(data[index]);
    }
}

void tset_color(enum color fg, enum color bg){
    tcolor = vga_entry_color(fg, bg);
}
