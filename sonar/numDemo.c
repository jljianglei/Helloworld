#include<stdio.h>
int main() {
   unsigned int a = 0xff000000;
   printf("%u\n",a);
   printf("%d\n",a);
   int b = (int)a;
   printf("b = %d\n",b);
   unsigned int c = 0x4fff0000;
   int cc = (int)c;
   printf("c = %u,cc = %d\n",c,cc);
   int i = -10;
   unsigned int ii = (unsigned int)i;
   printf("i = %d,ii = %u\n",i,ii);
   int j = 1 << 16;
   printf("0x%x\n",j);
   return 0;
}
