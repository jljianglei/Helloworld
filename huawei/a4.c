#include<stdio.h>
#define SQUARE(a) ((a) * (a))
int main() {
    int a = 5;
	int b = SQUARE(a++);
	printf("a = %d,b = %d\n",a,b);
	return 0;
}
