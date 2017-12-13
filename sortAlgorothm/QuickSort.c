/*
 *快速排序：基本思想是通过一趟排序将要排序的数据分割成独立的2部分，其中一部分的所有数据
 *都比另一部分都要小，然后再按此方法对这2部分数据分别进行快速排序（递归），以此达到使整个数据变成
 *有序序列。
 *一趟排序的算法是:
 *(1)设置2个变量i,j,排序的时候，i = 0,j = N - 1;
 *(2)以第一个数组元素作为关键元素，赋值给key，即key = a[0];
 *(3)j开始向前搜,找到第一个小于key的值a[j],将a[j]和a[i]交换；
 *(4)i开始向后搜，找到第一个比key大的数，将a[i]和a[j]交换；
 *（5）如果i不等于j，重复（3），（4）步骤；如果i等于j，则递归排序2个子部分
 *这个算法的问题在于使用了递归，如果n的规模太大，本人测试了下，当n达到200000的时候，函数调用栈就溢出了，出现断错误，所以还是要
 *改进的
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
/*
 *param
 *     a: 指向数组首元素的地址
 *     low: 数组第一个元素的下标
 *     high: 数组最后一个元素的下标
 */
void Qsort(int * a,int low,int high) {
    if(low >= high) {
	   return;
	}
    int first = low;
	int last = high;
	int key = a[first];
	while(first < last) {
	    /*last从后向前搜，如果搜到比key小的，和fisrt交换*/
		/*如果'>='的=去掉，会出现死循环？*/
		while(a[last] >= key && (last > first))
			last--;
		/*存在别key小的数*/
		if(last > first)
			a[first] = a[last];
		/*fisrt从前往后搜，如果找到比key大的，则和last交换*/
		while(a[first] <= key && (first < last))
			first++;
		/*存在别key大的数*/
		if(first < last)
			a[last] = a[first];
	}
	/*一趟完后，first指向枢轴的位置*/
	a[first] = key;
	/*递归排趟出来的2个部分*/
	Qsort(a,low,first - 1);
	Qsort(a,first + 1,high);
}
void printArr(int * a,int length) {
     int i;
	 for(i = 0; i < length; ++i) {
	     printf("%d ",*(a + i));
	 }
	 printf("\n");
}
/*length个数中第k大的数，数组是已经排序好的，升序，然后可以求任意第k大的数，注意k的取值*/
void kBigOfN(int * a,int length,int k) {
	 if(k < 1) {
	    printf("error k,%d\n",k);
		return;
	 }
	 if(k == 1) {
	    printf("第%d大的数是%d\n",k,*(a + length - 1));
		return;
	 }
     int count = 1;
	 int j;
	 for(j = length-2; j >= 0; j--) {
	     if(a[j] != a[j - 1]) {
			 count++;
			 if(count == k) {
			    break;
			 }
		 }
	 }
	 printf("第%d大的数是%d\n",k,a[j]);
}
int main() {
    int a[] = {54,54,54,61,59,52,1,0,71,23,99,33,24};
	int length = sizeof(a)/sizeof(a[0]);
	printf("a length  = %d\n",sizeof(a)/sizeof(a[0]));
	struct timeval t1,t2;
	int timeSub = 0;
	printArr(a,sizeof(a)/sizeof(a[0]));
	gettimeofday(&t1,NULL);
	Qsort(a,0,sizeof(a)/sizeof(a[0]) - 1);
	gettimeofday(&t2,NULL);
	timeSub = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
	printf("sort time is %d(ms)\n",timeSub);
	kBigOfN(a,length,4);
	kBigOfN(a,length,6);
	kBigOfN(a,length,1);
	kBigOfN(a,length,0);
	kBigOfN(a,length,19);
	printArr(a,sizeof(a)/sizeof(a[0]));
	return 0;
}
