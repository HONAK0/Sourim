#include "drivers/port.h"
#include "screen/terminal.h"

#define bit(n) (1<<(n))
#define check_flag(flags, n) ((flags) & bit(n))

void reboot(){
    clear();
    print("Reboot...");
    uint8_t v;
    asm volatile ("cli");
    do
    {
        v = port_byte_in(0x64);
        if (check_flag(v, 0) != 0)
            port_byte_in(0x60);
    } while (check_flag(v, 1) != 0);
 
    port_byte_out(0x64, 0xfe);
loop:
    asm volatile ("hlt");
    goto loop;
}
void shutdown(){
    clear();
    print("Shutdown...");
    port_word_out(0xB004, 0x2000);
    port_word_out(0x604, 0x2000);
    port_word_out(0x4004, 0x3400);
}
