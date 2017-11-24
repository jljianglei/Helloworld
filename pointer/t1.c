#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main() {
    int a[5] = {1,2,3,4,5};
	int *p = (int *)(&a + 1);/*因为a是个数组名，所以&a是个数组指针,&a+1会根据数组的长度来来调整+1的实际长度，所以&a+1得到的地址
							  *数值上是a[5]的地址，但是含义是不一样的,&a+1还是数组指针,a[5]的地址是个整形指针,再+1步长是不一样的
							  *
							  */
	printf("%d,%d\n",*(a + 1),*(p - 1));
	return 0;
}
