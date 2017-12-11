/*
 *1 到n 1出现的次数
 *
 *
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
int numberOf1(unsigned int n) {
    int counts = 0;
	while(n) {
	   if(n % 10 == 1) {
	      counts++;
	   }
	   n /= 10;
	}
	return counts;
}
int numberOf1Between1AndN(unsigned int n) {
    long number = 0;
	unsigned int i;
	for(i = 1; i < n; ++i) 
		number += numberOf1(i);
	return number;
}
int main() {
    struct timeval oldTimer,newTimer;
	int timeSub = 0;
	gettimeofday(&oldTimer,NULL);
	long num = numberOf1Between1AndN(100000000);
	gettimeofday(&newTimer,NULL);
	timeSub = (newTimer.tv_sec - oldTimer.tv_sec) * 1000 + (newTimer.tv_usec - oldTimer.tv_usec) / 1000;
	printf("timeSub %d(ms)\n",timeSub);
	printf("number = %ld\n",num);
	return 0;
}
