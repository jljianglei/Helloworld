/*
 *斐波那契查找
 *
 *
 *
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE 20

void Qsort(int * a,int low,int high) {
	if(low >= high)
		return;
    int first,last;
	int key = a[low];
	first = low;
	last = high;
	while(first < last) {
	     while(first < last && (a[last] >= key))
			 last--;
		 a[first] = a[last];
		 while(first < last && (a[first] <= key))
			 first++;
		 a[last] = a[first];
	}
	a[first] = key;
	Qsort(a,low,first - 1);
	Qsort(a,first + 1,high);
}
/*构造一个斐波那契数组*/
void Fibonacci(int * a) {
   *a = 1;
   *(a + 1) = 1;
   int i;
   for(i = 2; i < MAX_SIZE; ++i) {
       *(a + i) = *(a + i - 1) + *(a + i -2);
   }
}
/**/
int FibonacciSearch(int *a,int n,int key) {
    int low = 0;
	int high = n - 1;
	int F[MAX_SIZE];
	Fibonacci(F);
	int k = 0;
	/*计算n在斐波那契数列的位置*/
	while(n > F[k] - 1)
		k++;
	/*将数组扩展到F[k] - 1的长度*/
	int * tmp;
	tmp = malloc(F[k] - 1);
	memcpy(tmp,a,n*sizeof(int));
    int i;
	for(i = n; i < F[k] - 1; ++i) {
	    tmp[i] = a[n - 1];
	}

	/*查找*/
	while(low <= high) {
	    int mid = low + F[k - 1] - 1;
		if(key == tmp[mid]) {
		  if(mid < n) {
		     return mid;
		  }
		  else {
		     return n - 1;
		  }
		}
		else if(key < tmp[mid]) {
		   high = mid - 1;
		   k -= 1;
		}
		else if(key > tmp[mid]) {
		   low = mid + 1;
		   k -= 2;
		}
	}
	if(tmp) {
	   free(tmp);
	   tmp = NULL;
	}
	return -1;
}
void print(int * a,int length) {
   int i;
   printf("a elem is :\n");
   for(i = 0; i < length; ++i) {
       printf("%d ",a[i]);
   }
   printf("\n");
}
int main() {
   int a[] = {1,5,10,45,23,18,35,90,4,61,39,100,0,72};
   int length = sizeof(a)/sizeof(a[0]);
   printf("length = %d\n",length);
   print(a,length);
   Qsort(a,0,length  -1);
   print(a,length);
   int ret;
   int key = 35;
   ret = FibonacciSearch(a,length,key);
   if(ret == -1) {
      printf("not find %d\n",key);
   }
   else {
      printf("find it %d,index of arr is %d\n",key,ret);
   }
   key = 100;
   ret = FibonacciSearch(a,length,key);
   if(ret == -1) {
      printf("not find %d\n",key);
   }
   else {
      printf("find it %d,index of arr is %d\n",key,ret);
   }
   key = 0;
   ret = FibonacciSearch(a,length,key);
   if(ret == -1) {
      printf("not find %d\n",key);
   }
   else {
      printf("find it %d,index of arr is %d\n",key,ret);
   }
   return 0;
}
