#include<stdio.h>
#include<stdlib.h>
int main() {
    int *p;
	p = (int*)malloc(0);
	printf("p = %p\n",p);
	*p = 10;
	printf("*p = %d\n",*p);
	return 0;
}
