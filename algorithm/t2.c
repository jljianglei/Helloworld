#include<stdio.h>
/*
 *问题：
 *求2个数的最大公约数
 *算法：
 *辗转相除法，用较大的数除以较小的数，如果能整除，则较小的数就是最大公约数，
 *如果不能整除，就把余数作较小的数，继续除
 */

int func(int a,int b) {
	int m = 0;
     do{
        m = a % b;
       if(m == 0)
		   return b;
	   a = b;
	   b = m;
	 }while(b != 0);
}
int main() {
    int a = 10,b = 3;
	printf("a = %d,b = %d, %d\n",a,b,func(a,b));
	return 0;
}
