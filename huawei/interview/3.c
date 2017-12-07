/*
 *数值的整数此方，有几个细节需要注意
 *1底数为0指数为负的情况，这属于非法输入，可以定义全局变量保存这个errcode
 *2底数不为0，指数为负的情况，需要对指数求绝对值，算出结果后，再取倒数
 *3判断2个double是否相等，不能直接用==，而是作减法，看范围是不是在一个很小的范围内，-0.000001 ～ 0.000001差不多。
 *4乘方有快速相乘的方法，用递归可以实现，属于性能优化，不一定说就要
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define bool int
#define true 1
#define false 0
static bool validInput = true;
bool isEqual(double b1,double b2) {
   if((b1 - b2) > -0.00001 && (b1 - b2) < 0.00001)
	   return true;
   return false;
}
double power(double base,int exponent) {
  double result = 1;
  if(isEqual(base,0.0) && (exponent < 0)) {
     validInput = false;
	 return result;
  }
  unsigned int tmp = (unsigned int)exponent;
  if(exponent < 0) {
    tmp = (unsigned int)(-exponent); 
  }
  int i;
  for(i = 0; i < tmp; ++i)
	  result *= base;
  if(exponent < 0) {
     result = 1.0/result;
  }
  return result;
}
int main() {
   double r1 = power(0,-1);
   printf("validInput = %d\n",validInput);
   r1 = power(0,2);
   printf("r1 = %.2lf\n",r1);
   r1 = power(0,-3);
   printf("r1 = %.2lf\n",r1);
   r1 = power(0,0);
   printf("r1 = %.2lf\n",r1);
   r1 = power(-2,2);
   printf("r1 = %.2lf\n",r1);
   r1 = power(3,3);
   printf("r1 = %.2lf\n",r1);
   r1 = power(2,-2);
   printf("r1 = %.2lf\n",r1);
   return 0;
}
