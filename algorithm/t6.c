/*
 *问题：一串二进制，判断其中有几个1
 *第一种解法，如果n是一个负数的话，则最高位是1，右移操作的话，左边补的是1不是0，最终会右移成0xffffffff,形成死循环
 *第二种解法可以规避这个问题，但是每次都需要右移32次，flag才会移成0
 *第三种解法，利用了一个规律，一个整数减去1，都是把它最右边的1变成0，如果1的右边有0的话，0全变成1，1的左边位保持不变
 *，如果再和这个整数做与运算的话，就相当于把最右边的1变成0，重复上述操作，这个整数就会变成0
 */
#include<stdio.h>
#define METHOD3
int numberOf1method1(int n) {
    int count = 0;
	while(n != 0) {
	   if(n & 0x1) {
	      count++;
	   }
	   n >>= 1;
	   printf("0x%x\n",n);
	}
	return count;
}
int numberOf1method2(int n) {
    int count = 0;
	int flag = 1;
	while(flag) {
	    if(n & flag) {
		   count++;
		}
		flag <<= 1;
		printf("0x%x\n",flag);
	}
	return count;
}
int numberOf1method3(int n) {
    int count = 0;
	while(n) {
	   count++;
	   n = (n - 1) & n;
	   printf("0x%x\n",n);
	}
	return count;
}
int main() {
   int t1 = 0xFF000000;
   int count;
#ifdef METHOD1
   count = numberOf1method1(t1);
#elif defined(METHOD2)
   count = numberOf1method2(t1);
#else
   count = numberOf1method3(t1);
#endif
   printf("t1 0x%x,count = %d\n",t1,count);
   return 0;
}
