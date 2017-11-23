#include<stdio.h>
int main() {
    void * p = malloc(10);
	int a;
	int b[6] = {0};
	int (*ptr)[6];
	ptr = &b;
	printf("&ptr %p\n",&ptr);
	printf("&a = %p\n",&a);
	printf("&b = %p\n",&b);
	printf("&p = %p,p = %p\n",&p,p);
	return 0;
}
