#include<stdio.h>
int main() {
    int a[] = {1,2,3,4,5};
	int *p = a;
	*(p++) = 10;
	printf("%d,%d\n",*p,*a);
	return 0;
}
