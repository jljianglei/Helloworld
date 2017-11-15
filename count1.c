#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SECONDS_PER_YEAR (365*24*60*60)UL
#define dprint(exp) printf(#exp" = %d\n",exp)
int main() {
     int count = 0;
	 int num = 0x11011011,tmp = num;
	 while(num) {
	    num &= num -1;
		count++;
	 }
	 printf("num = 0x%x,counts of 1 is %d\n",tmp,count);
	 int a = 20,b = 10;
	 dprint(a/b);
	 int c[3][2] = {{1,2},{3,4},{5,6}};
	 int * p = c[1];
	 printf("*p %d,*(p + 1) %d\n",*p,*(p + 1));
Loop:
	 printf("hello\n");
	 sleep(2);
	 goto Loop;
	 return 0;
}
