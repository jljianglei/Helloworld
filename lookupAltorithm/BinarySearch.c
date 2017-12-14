/*
 *二分查找：元素必须是有序的，如果是无序的则要先进行排序操作
 *基本思想：也称为折半查找，用给定值和中间节点的关键字进行比较，如果相等，则查找结束。
 *如果不相等，那么如果给定值小于中间节点，那么在前半区找，如果大于中间值，在后面找
 *有点像猜商品的价格，给定一个价格区间，我们会先猜中间的值，如果猜中结束，如果不是，再问大了还是小了，从而减少
 *范围区间。
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*快速排序和冒泡排序有些类似，都属于交换排序*/
void Qsort(int * a,int low,int high) {
	 /*递归结束的条件，没有就出不来了，最终会栈溢出*/
	 if(low >= high)
		 return;
	 int first,last;
	 first = low;
	 last = high;
	 int key = *(a + low);
	 while(first < last) {
	     /*先从last向前搜，找到比key小的（注意不包括等于的）,则交换first和last的值*/
		 while(first < last && (*(a + last) >= key))
			 last--;
		 *(a + first) = *(a + last);
		 /*从first开始向后搜，找到比key大的(注意也不包括等于的),也交换first和last的值*/
		 while(first < last && (*(a + first) <= key))
			 first++;
		 *(a + last) = *(a + first);
	 }
	 *(a + first) = key;
	 Qsort(a,low,first - 1);
	 Qsort(a,first + 1,high);
}

int BinarySearch(int * a,int key,int length) {
    int low = 0;
	int high = length - 1;
	while(low <= high) {
	   int mid = (low + high) / 2;
	   if(a[mid] == key)
		   return mid;
	   else if(a[mid] > key) {
	      high = mid - 1;
	   }
	   else if(a[mid] < key) {
	      low = mid + 1;
	   }
	}
	return -1;
}
void print(int * a,int length) {
   int i;
   printf("a elem is:\n");
   for(i = 0; i < length; ++i) {
       printf("%d ",*(a + i));
   }
   printf("\n");
}
int main() {
    int a[] = {3,45,23,16,75,43,20,90,0,5,19,56,100,77,23};
	int length = sizeof(a)/sizeof(a[0]);
	printf("length = %d\n",length);
    print(a,length);
	Qsort(a,0,length - 1);
	print(a,length);
	int key = 11;
	int ret = BinarySearch(a,key,length);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find it %d,index of array is %d\n",key,ret);
	}
	key = 0;
	ret = BinarySearch(a,key,length);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find it %d,index of array is %d\n",key,ret);
	}
	key = 90;
	ret = BinarySearch(a,key,length);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find it %d,index of array is %d\n",key,ret);
	}
	key = 23;
	ret = BinarySearch(a,key,length);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find it %d,index of array is %d\n",key,ret);
	}
	key = 100;
	ret = BinarySearch(a,key,length);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find it %d,index of array is %d\n",key,ret);
	}
	return 0;
}
