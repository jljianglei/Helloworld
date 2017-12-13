/*
 *冒泡排序：从头开始，重复的比较2个相邻的元素，如果前一个元素别后面的元素要大，则交换2个元素的值，
 *这样一趟下来，最大的数就放在了最后一个位置，然后从新从头开始，因为最后一个数已经有序，所以比较次数减1，
 *以此重复。
 *算法的改进在于
 *1设置1个标志位，记录每趟最后一次交换的位置，则下一趟扫描到该位置即可，可以减少外循环的次数
 *2一趟找出一个最大值和最小值，也可以减少外循环的次数
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
void bubbleSort1(int * a,int length) {
    int i,j;
	int tmp;
	for(i = 0; i < length; ++i) {
	    for(j = 0; j < length - 1 - i; j++) {
		    if(a[j] > a[j + 1]) {
			   tmp = a[j];
			   a[j] = a[j + 1];
			   a[j + 1] = tmp;
			}
		}
	}
}

/*设置变量pos记录每趟最后一次交换的位置*/
void bubbleSort2(int * a,int length) {
    int i = length -1;
	int pos;
	int j;
	int tmp;
	while(i > 0) {
	   pos = 0;
	   for(j = 0; j < i; ++j) {
	       if(a[j] > a[j + 1]) {
		      tmp = a[j];
			  a[j] = a[j + 1];
			  a[j + 1] = tmp;
		   }
		   pos = j;
	   }
	   i = pos;/*为下一趟排序作准备*/
	}
}
/*一次求2个值，最大值和最小值*/

void bubbleSort3(int * a,int length) {
    int low = 0;
	int high = length - 1;
	int tmp;
	int j;
	while(low < high) {
	    for(j = 0; j < high; ++j) {
		    if(a[j] > a[j + 1]) {
			   tmp = a[j];
			   a[j] = a[j + 1];
			   a[j + 1] = tmp;
			}
		}
		--high;
		for(j = high; j > low; j--) {
		   if(a[j] < a[j - 1]) {
		      tmp = a[j];
			  a[j] = a[j - 1];
			  a[j - 1] = tmp;
		   }
		}
		low++;
	}
}
void printArr(int * a,int length) {
    int i;
	for(i = 0; i < length; ++i) {
	    printf("%d ",*(a + i));
	}
	printf("\n\n\n");
}
int main() {
    int a[200000] = {3,44,38,5,47,15,36,36,2,46,19,50};
	int length = sizeof(a)/sizeof(a[0]);
	struct timeval t1,t2;
	int timeSub = 0;
	printf("a length %d\n",length);
    //printArr(a,length);
	//bubbleSort1(a,length);
	//bubbleSort2(a,length);
    gettimeofday(&t1,NULL);	
	bubbleSort1(a,length);
	gettimeofday(&t2,NULL);
	timeSub = (t2.tv_sec - t1.tv_sec)* 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
	//printArr(a,length);
	printf("timeSub = %d(ms)\n",timeSub);
	return 0;
}
