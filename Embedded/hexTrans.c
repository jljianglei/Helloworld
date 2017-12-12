#include<stdio.h>
/*10进制=八进制/10*8 + 八进制%10*/
unsigned int oct2dec(unsigned int oct) {
   return oct / 10 * 8 + oct % 10;
}
int main() {
   unsigned int a = 0775;
   printf("a = %o\n",a);
   a = oct2dec(a);
   printf("a = %d\n",a);
   return 0;
}
