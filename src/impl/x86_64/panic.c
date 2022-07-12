#include "terminal.h"

void panic(char *reason) {
  print("The system kernel has detected an unrecoverable critical error\n")
  print("Error: ")
  print(reason)
  print("\nKernel suspended")
}
