#include "terminal.h"
#include "drivers/keyboard.h"
#include "str.h"
#include "config.h"

int timer;

void kstart(){
    print("Welcome!\n\n");
}
void krun(){
    tputchar('\n');
    char *input = keyboard_input("> ");
    print(input);
}
