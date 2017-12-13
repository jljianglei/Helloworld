/*
 *选择排序：也是一种简单直观的排序算法。它的工作原理是
 *首先在未排序的序列中找出最大（小）元素，存放于有序序列的起始位置，
 *然后，再从剩余未排序的序列中寻找最小（大）的元素，存放于已排序序列的末尾，直到所有元素排列完毕
 *N元素的数组a[N]最多选择N - 1趟，就排序好了，最后一趟选择一次就行了
 *
 *
 *
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void selectSort(int * a,int length) {
    int minIndex;
	int tmp;
	int i,j;
	/*最多N - 1*/
	for(i = 0; i < length - 1; ++i) {
	    minIndex = i;
		for(j = i + 1; j < length; ++j) {
		    if(a[j] < a[minIndex])
				minIndex = j;
		}
		tmp = a[i];
		a[i] = a[minIndex];
		a[minIndex] = tmp;
	}
}
void printArr(int * a,int length) {
    int i;
	for(i = 0; i < length; ++i) {
	    printf("%d ",*(a + i));
	}
	printf("\n");
}
int main() {
    int a[200000] = {3,44,38,5,47,15,36,26,27,2,46,4,19,50,48};
	int length = sizeof(a)/sizeof(a[0]);
	printf("a length %d\n",length);
	struct timeval t1,t2;
	int timeSub = 0;
	gettimeofday(&t1,NULL);
	//printArr(a,length);
	selectSort(a,length);
	gettimeofday(&t2,NULL);
	timeSub = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
	printf("timeSub = %d(ms)\n",timeSub);
	//printArr(a,length);
	return 0;
}
