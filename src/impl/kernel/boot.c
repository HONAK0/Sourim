#include "main.h"
#include "terminal.h"

void boot(){
    tset_color(WHITE, BLACK);
    clear();
    tsetcur(1,0);
    kstart();
    while(1){
        krun();
    }
}