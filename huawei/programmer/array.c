#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*
 *数组中出现次数超过一半的数
 *数组中有一个数字出现的次数超过了数组长度的一半，如果把这个数组排序，那么排序之后
 *位于中间的数字一定就是那个出现次数超过数组长度一半的数字。
 *也就是统计学上的中位数,长度为n的数组中第n/2大的数字
 *有成熟的算法可以得到数组中任意第k大的数字
 */

static bool g_inputInValid = false;
bool checkInValidArray(int * numbers,int length) {
     g_inputInValid = false;
	 if(numbers == NULL && length <= 0)
		 g_inputInValid = true;
	 return g_inputInValid;
}

bool checkMoreThanHalf(int * numbers,int length,int number) {
     int times = 0;
	 int i;
	 for(i = 0; i < length; ++i) {
	     if(numbers[i] == number)
			 times++;
	 }
	 bool isMore = true;
	 if(times * 2 <= length)
		 isMore = false;
	 return isMore;
}
int moreThanHalfNum(int * numbers,int length) {
     if(checkInValidArray(numbers,length))
		 return 0;
	 int middle = length >> 2;
	 int start = 0;
	 int end = length - 1;
}


