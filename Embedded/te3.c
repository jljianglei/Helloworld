#include<stdio.h>
int main() {
   unsigned short a = 0xabcd;
   printf("a = 0x%x\n",a);
   unsigned short lowAddr = a & 0xff;
   unsigned short highAddr = (a >> 8) & 0xff;
   unsigned short b = (lowAddr << 8) | highAddr;
   printf("b = 0x%x\n",b);
   unsigned char* p = (unsigned char*)&a;
   *p = *p ^ (*(p + 1));
   *(p + 1) = *p ^ (*(p + 1));
   *p = *p ^ (*(p + 1));
   printf("a = 0x%x\n",a);
   return 0;
}
