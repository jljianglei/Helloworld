#include<stdio.h>
int square(volatile int * ptr) {
    return (*ptr) * (*ptr);
}
int main() {
    int a = 5;
	int result = square(&a);
	printf("result = %d\n",result);
	return 0;
}
