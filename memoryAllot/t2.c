#include<stdio.h>
void func(void) {
   func();
}
int main() {
   func();
   return 0;
}
