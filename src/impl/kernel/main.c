#include "terminal.h"
#include "drivers/keyboard.h"
#include "str.h"
#include "user_files/pcmem.h"

#define NAMEOS "Sourim"
#define VERSIONOS "1.0"

void kstart(){
    print("Welcome!\nOS: ");
    print(NAMEOS);
    print("\nVersion Kernel: ");
    print(VERSIONOS);
    print("\n\n");
}

void print_log(enum color log_color, char *log, char *message){
    print("[");
    tset_color(log_color, BLACK);
    print(log);
    tset_color(WHITE, BLACK);
    print("] ");
    print(message);
}

void krun(){
    char enter_input[512];
    tset_color(GREEN, BLACK);
    print(namepc);
    print("@");
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
        } else {
            print_log(RED, "ERROR", "Arguments is empty\n");
        }
    } else if(strcheck(command, "help", 32)){
        print("echo <text*> - Print the text in terminal\n");
        print("clear - Clear screen\n");
        print("setusername <name*> - Set username\n");
    } else if(strcheck(command, "clear", 32)){
        clear();
    } else if(strcheck(command, "setusername", 32)){
        if(strlen(enter_input) > strlen(command)+1){
            int ii = 0;
            for(int i = strlen(command)+1; i < strlen(command)+33; i++){
                username[ii] = enter_input[i];
                ii++;
            }
            print_log(GREEN, "OK", "Username is changed\n");
        } else {
            print_log(RED, "ERROR", "Arguments is empty\n");
        }
    } else {
        print_log(CYAN, "WARN", "Unknown command, please type 'help'\n");
    }
}
