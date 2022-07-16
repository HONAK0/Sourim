#include <stdint.h>
#include "drivers/port.h"
#include "drivers/ports.h"
#include "screen/terminal.h"
#include "panic.h"
#include "str.h"
#define input_max_size 512

int capslock = 0;
static char *keyboard_char_keys_down[] ={"ENTER","q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "1", "2", "3", "4", "5",
"6", "7", "8", "9", "0", "-", "=", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "\\","z", "x", "c", "v", "b", "n", "m",
",", ".", "/", " ", "\1"};
static char *keyboard_char_keys_up[] =  {"ENTER","Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "1", "2", "3", "4", "5",
"6", "7", "8", "9", "0", "-", "=", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "\\","Z", "X", "C", "V", "B", "N", "M",
",", ".", "/", " ", "\1"};
static uint16_t keyboard_byte_keys[] =  {0x1c,   0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x02,0x03,0x04,0x05,0x06,
0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,
0x33,0x34,0x35,0x39,0x0e};

static char *keyboard_char_keys_shift[] =     {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+", "|", ":", "\"","~", "<", ">", "?", "{", "}"};
static uint16_t keyboard_byte_keys_shift[] =  {0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x2b,0x27,0x28,0x29,0x33,0x34,0x35,0x1a,0x1b};
int capslock_pressed = 0;
int nullkey = 0;
int enterpressed = 0;
char input[input_max_size];
uint16_t oldkey;
int shift_pressed = 0;

uint16_t keyboard_byte_key(){
    uint16_t key = port_byte_in(KEYBOARD_PS2_PORT);
    if(key == 0x3a && oldkey != 0x3a){
        if(capslock_pressed){
            capslock_pressed = 0;
        } else {
            capslock_pressed = 1;
        }
    }
    if(key == 0xaa){
        shift_pressed = 0;
    } else if(key == 0x2a){
        shift_pressed = 1;
    }
    oldkey = key;
    return key;
}
char *keyboard_key(){
    int i = 0;
    uint16_t key = keyboard_byte_key();
    while(keyboard_byte_keys[i] != 0){
        if(key == keyboard_byte_keys_shift[i] && shift_pressed){
            nullkey = 0;
            return keyboard_char_keys_shift[i]; 
        }
        if(key == keyboard_byte_keys[i]){
            nullkey = 0;
            if(capslock_pressed){
                return keyboard_char_keys_up[i];
            } else {
                return keyboard_char_keys_down[i];
            }
        }
        i++;
    }
    nullkey = 1;
    return (char*)"null";
}
void tkeyboard_input(){
    int timer = 0;
    int len = 0;
    memset(input, '\0', 32);
    while(1){
        char *key = keyboard_key();
        if(timer == 0){
            if(key == (char*)"ENTER" && enterpressed == 0){
                print("\n");
                enterpressed = 1;
                break;
            } else if(key != (char*)"ENTER"){
                enterpressed = 0;
            }
            if(key == (char*)"\1" && len > 0){
                input[strlen(input)-1] = 0;
                len--;
                print(key);
                timer = 1;
            } else if(strlen(key) == 1 && key != (char*)"\1" && len < input_max_size){
                charadd(input, key[0]);
                len++;
                print(key);
                timer = 1;
            }
        }
        if(key[0] == 'n' && key[strlen(key)-1] == 'l'){
            timer = 0;
        }
    }
}
void keyboard_input(char tinput[]){
    memset(tinput, '\0', 32);
    tkeyboard_input();
    for(int i = 0; i < input_max_size; i++){
        tinput[i] = input[i];
    }
}
