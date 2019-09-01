#include "main.h"

void cause_segfault() {
  int x = *(int*)0x4; printf("%d\n" ,x);
}
