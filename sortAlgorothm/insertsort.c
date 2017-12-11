/*
 *插入排序：把一个新的数据插入到一个有序的数组中，并继续保持有序，例如有一个长度为N的无序数组
 *，进行N-1次的插入即可完成排序。
 *
 *
 *
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void print(void * data,int length);
void insertSort(int * data,int length) {
   if(length <= 1) {
      return;
   }
   int i;
   for(i = 1; i < length; ++i) {
       int j = 0;
	   /*从前向后找，看当前数据即data[i]的前面有没有比它大的数，如果有，那么第一个比他大的数就是他的插入位置*/
	   while(j < i && (data[j] < data[i]))
		   j++;
	   /*j < i说明data[j]比data[i]大，那么从j到i-1的数据都要向后移，j就是新的数据的插入位置*/
	   if(j < i) {
	     int k = i;
		 int tmp = data[i];
		 while(k > j) {
		   data[k] = data[k - 1];
		   k--;
		 }
		 data[j] = tmp;
	   }
	   print(data,length);
   }
}

/*还可以从后向前找位置*/
void insertSort2(int * data,int length) {
   if(length <= 1) {
      return;
   }
   int i;
   for(i = 1; i < length; ++i) {
       int j = i - 1;
	   int tmp = data[i];
	   while(j >= 0 && (data[j] > tmp)) {
		   data[j + 1] = data[j];
		   j--;
	   }
	   /*说明存在比data[i]大的数,此时data[j] < data[i],所以j + 1是它的插入位置*/
	   if(j != (i -1))
		   data[j + 1] = tmp;
	   print(data,length);
   }
}
/*由于数组是有序的，所以查找位置可以使用2分查找*/

int findInsertIndex(int* data,int length,int iData) {
    int beginIndex = 0;
	int endIndex = length - 1; 
	int index = -1;
	while(beginIndex <= endIndex) {
		index = (beginIndex + endIndex) / 2;
		if(iData > data[index])
			beginIndex = index + 1;
		else
            endIndex = index - 1;
	}
	if(iData >= data[index])
		index++;
	return index;
}
void insertSort3(int * data,int length) {
   if(length <= 1) 
	   return;
   int i;
   for(i = 1; i < length; ++i) {
      int index = findInsertIndex(data,length,data[i]);
	  if(index != i) {
	     int j = i;
		 int tmp = data[i];
		 while(j > index) {
		    data[j] = data[j - 1];
			j--;
		 }
		 data[index] = tmp;
	  }
	  print(data,length);
   }
}
void print(void * data,int length) {
   int i;
   for(i = 0; i < length; ++i) {
       printf("%d ",*((int*)data + i));
   }
   printf("\n");
}
int main() {
    int a[] = {1,20,5,30,27,60,45};
	//insertSort(a,sizeof(a)/sizeof(int));
	//insertSort2(a,sizeof(a)/sizeof(int));
	insertSort3(a,sizeof(a)/sizeof(int));
	return 0;
}
