#include<stdio.h>
#define TEST
#ifndef TEST
#error ERROR: not define TEST
#endif
int main() {
   return 0;
}
