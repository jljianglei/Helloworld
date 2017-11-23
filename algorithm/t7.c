/*
 *问题：数值的整数次方
 *
 *
 *
 *
 */
#include<stdio.h>
static double mypow(double base,int num) {
    double result = 1.0;
	int i;
	int tmp = num;
	if(base == 0 && (num < 0)) {
	   return 0;
	}
	if(num < 0)
		tmp = (-num);
	for(i = 0; i < tmp; ++i) {
	    result *= base;
	}
	if(num < 0) {
	  result = 1.0/result;
	}
	return result;
}
int main() {
    double b1 = 10;
	double result = mypow(b1,2);
	printf("b1 = %f,result = %f\n",b1,result);
	b1 = 0;
	result = mypow(b1,2);
	printf("b1 = %f,result = %f\n",b1,result);
	b1 = 0;
	result = mypow(b1,2);
	printf("b1 = %f,result = %f\n",b1,result);
	b1 = 10;
	result = mypow(b1,-2);
	printf("b1 = %f,result = %f\n",b1,result);
	b1 = -2;
	result = mypow(b1,-2);
	printf("b1 = %f,result = %f\n",b1,result);
	b1 = 0;
	result = mypow(b1,-2);
	printf("b1 = %f,result = %f\n",b1,result);
	b1 = -2;
	result = mypow(b1,3);
	printf("b1 = %f,result = %f\n",b1,result);
	return 0;
}
