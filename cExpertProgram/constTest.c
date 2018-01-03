#include<stdio.h>
int main() {
    const int a = 10;
	printf("&a = %p\n",&a);
	printf("a = %d\n",a);
	//a = 12;
	int * p = (int *)&a;
	*p = 11;
	printf("a = %d\n",a);
	return 0;
}
