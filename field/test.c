#include<stdio.h>
void func() {
   const int a = 10;
   printf("a = %d\n",a);
}
int main() {
   func();
   func();
   return 0;
}
