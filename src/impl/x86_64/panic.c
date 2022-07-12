#include "screen/terminal.h"

void show_panic(char *error){
    tset_color(RED, BLACK);
    print("CRITICAL ERROR > ");
    tset_color(WHITE, BLACK);
    print(error);
}