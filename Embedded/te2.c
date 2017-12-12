#include<stdio.h>
#define BIT_MASK(bit_pos) (0x01 << (bit_pos))

int bit_reset(unsigned int * val,unsigned int pos) {
    if(pos >= sizeof(unsigned int))
		return 0;
	*val = (*val) & ~BIT_MASK(pos);
}
int main() {
   unsigned int x = 0xffffffff;/*x有32位，但是pos的取值从0开始，所以有效范围是0~31,这点务必注意*/
   printf("x = %x\n",x);
   bit_reset(&x,0);
   printf("x = %x\n",x);
   return 0;
}
