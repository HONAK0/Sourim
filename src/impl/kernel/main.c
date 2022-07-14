#include "screen/terminal.h"
#include "drivers/keyboard.h"
#include "str.h"
#include "drivers/port.h"
#include "user_files/pcmem.h"
#include "contol/power.h"
#include "drivers/com_ports.h"
#include "panic.h"
#include "kernel.h"
#include "screen/vga.h"

#define OSNAME "Sourim"
#define OSVERSION "1.0.7"

char *logo[] = {"           /////          ",
                "       /////////////      ",
                "  //////////////////////  ",
                "//////////////////////////",
                "///////////.....//////////",
                "/////////..///////////////",
                "//////////......//////////",
                "///////////////../////////",
                "///////////....///////////",
                "//////////////////////////",
                "  //////////////////////  ",
                "      //////////////      ",
                "          /////           "
};


void infosystem(int welcome){
    clear();
    tset_color(GREEN,BLACK);
    for(int i=0; i < 13; i++){
        print(logo[i]);
        tset_color(WHITE, BLACK);
        print("    ");
        switch(i){
            case 0:
                tset_color(LIGHT_CYAN,BLACK);
                tputchar('@');
                tset_color(LIGHT_GREEN,BLACK);
                print(username);
                tset_color(WHITE,BLACK);
                break;
            case 1:
                tset_color(LIGHT_CYAN,BLACK);
                print("OS: ");
                tset_color(WHITE,BLACK);
                print(OSNAME);
                break;
            case 2:
                tset_color(LIGHT_CYAN,BLACK);
                print("OS Version: ");
                tset_color(WHITE,BLACK);
                print(OSVERSION);
                break;
            case 3:
                tset_color(LIGHT_CYAN,BLACK);
                print("Kernel version: ");
                tset_color(WHITE,BLACK);
                print(KERNELVERSION);
                break;
            case 4:
                tset_color(LIGHT_CYAN,BLACK);
                print("Developers: ");
                tset_color(WHITE,BLACK);
                print("Honak(Andrew S.)");
                break;
            case 10:
                tset_color(CYAN,BLACK);
                print("Test colors:");
                tset_color(WHITE,BLACK);
                break;
            case 11:
                enum color colors1[] = {BLACK, RED,       GREEN,       BLUE,       CYAN,       BROWN,       MAGENTA};
                for(int clr=0; clr < 7; clr++){
                    tset_color(WHITE, colors1[clr]);
                    print("    ");
                }
                break;
            case 12:
                enum color colors2[] = {WHITE, LIGHT_RED, LIGHT_GREEN, LIGHT_BLUE, LIGHT_CYAN, LIGHT_BROWN, LIGHT_MAGENTA};
                for(int clr=0; clr < 7; clr++){
                    tset_color(WHITE, colors2[clr]);
                    print("    ");
                }
                break;
        }
        tset_color(GREEN, BLACK);
        tputchar('\n');
    }
    tset_color(WHITE,BLACK);
}

void print_log(enum color log_color, char *log, char *message, char *joiner){
    tset_color(log_color, BLACK);
    print(log);
    tset_color(WHITE, BLACK);
    print(joiner);
    print(message);
}

void kstart(){
    print_log(LIGHT_CYAN, "WAIT", "Init keyboard\n", " > ");
    if(port_byte_in(0x60 + 5) & 1){
        tputchar('\2');
        print_log(GREEN, "OK", "Init keyboard  \n", " > ");
    } else {
        tputchar('\2');
        print_log(RED, "WARN", "Init keyboard\n", " > ");
    }
    tset_color(LIGHT_CYAN, BLACK);
    print("\nWelcome to ");
    print(OSNAME);
    print("!\n\n");
    tset_color(WHITE, BLACK);
    //infosystem(1);
    //putpixel(1,1,4);
}

void krun(){
    char enter_input[512];
    tset_color(WHITE, BLACK);
    print("@");
    tset_color(CYAN, BLACK);
    print(username);
    tset_color(WHITE, BLACK);
    print(" $ ");
    keyboard_input(enter_input);
    char command[32];
    memset(command, 0, 32);
    for(int i = 0; i < 32; i++){
        if(enter_input[i] == ' ' || enter_input[i] == 0){
            break;
        }
        charadd(command, enter_input[i]);
    }
    
    if(strcheck(command, "echo", 32)){
        if(strlen(enter_input) > 5){
            for(int i = strlen(command)+1; i < strlen(enter_input); i++){            
                tputchar(enter_input[i]);
            }
            tputchar('\n');
        } else {
            print_log(RED, "ERROR", "Arguments is empty\n", " > ");
        }
    } else if(strcheck(command, "help", 32)){
        print_log(LIGHT_GREEN, "echo <text*>","        Print the text in terminal\n","");
        print_log(LIGHT_GREEN, "clear","               Clear screen\n",              "");
        print_log(LIGHT_GREEN, "setusername <name*>"," Set username\n",              "");
        print_log(LIGHT_GREEN, "russia","              Show Russian flag\n",         "");
        print_log(LIGHT_GREEN, "shutdown","            Power off\n",                 "");
        print_log(LIGHT_GREEN, "reboot","              Restart machine\n",           "");
    } else if(strcheck(command, "clear", 32)){
        clear();
    } else if(strcheck(command, "setusername", 32)){
        if(strlen(enter_input) > strlen(command)+1){
            int ii = 0;
            for(int i = strlen(command)+1; i < strlen(command)+33; i++){
                username[ii] = enter_input[i];
                ii++;
            }
            print_log(GREEN, "OK", "Username is changed\n", " > ");
        } else {
            print_log(RED, "ERROR", "Arguments is empty\n", " > ");
        }
    } else if(strcheck(command, "russia", 32)){
        for(int y = 0; y < 3; y++){
            for(int x = 0; x < 10; x++){
                if(y == 0){
                    tset_color(WHITE, WHITE);
                } else if(y == 1){
                    tset_color(WHITE, BLUE);
                } else if(y == 2){
                    tset_color(WHITE, RED);
                }
                tputchar(' ');
            }
            tputchar('\n');
        }
    } else if(strcheck(command, "reboot", 32)){
        reboot();
    } else if(strcheck(command, "shutdown", 32)){
        shutdown();
    } else if(strcheck(command, "checkports", 32)){
        print("COM1:");
        tputchar(com_read(COM1));
        print("\nCOM2:");
        tputchar(com_read(COM2));
        print("\nCOM3:");
        tputchar(com_read(COM3));
        print("\nCOM4:");
        tputchar(com_read(COM4));
        print("\nCOM5:");
        tputchar(com_read(COM5));
        print("\nCOM6:");
        tputchar(com_read(COM6));
        print("\nCOM7:");
        tputchar(com_read(COM7));
        print("\nCOM8:");
        tputchar(com_read(COM8));
        tputchar('\n');
    } else if(strcheck(command, "info", 32)){
        tputchar('\n');
        infosystem(0);
    } else {
        print_log(CYAN, "WARN", "Unknown command, please type 'help'\n", " > ");
    }
}
