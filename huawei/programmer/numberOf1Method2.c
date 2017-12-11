#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int powerBase10(unsigned int n);
int numberOf1Between1AndN(int n);
int numberOf1(const char* strN);
int numberOf1Between1AndN(int n) {
    if(n <= 0)
		return 0;
	char strN[50];
	sprintf(strN,"%d",n);
	return numberOf1(strN);
}
int numberOf1(const char* strN) {
    if(strN == NULL || *strN < '0' || *strN > '9' || *strN == '\0')
		return 0;
	int first = *strN - '0';
	unsigned int length = strlen(strN);
	if(length == 1 && first == 0)
		return 0;
	if(length == 1 && first > 0)
		return 1;
	int numFirstDigit = 0;
	if(first > 1)
		numFirstDigit = powerBase10(length - 1);
	else if(first == 1) {
	    numFirstDigit = atoi(strN + 1) + 1;
	}
	int numOtherDigits = first * (length - 1)*powerBase10(length  -2);
	int numRecu = numberOf1(strN + 1);
	return numFirstDigit + numOtherDigits + numRecu;
}
int powerBase10(unsigned int n) {
    int result = 1;
	unsigned int i = 0;
	for(i = 0; i < n; ++i) {
	   result *= 10;
	}
	return result;
}
int main() {
	struct timeval t1,t2;
	int timeSub = 0;
	gettimeofday(&t1,NULL);
	long num = numberOf1Between1AndN(100000000);
	gettimeofday(&t2,NULL);
	timeSub = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
	printf("timeSub = %d(ms)\n",timeSub);
	printf("num = %ld\n",num);
    return 0;
}
