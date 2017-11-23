#include<stdio.h>
int (*ptr)[6];
int main() {
    int a[6] = {1,2,3,4,5,6};
	ptr = &a;
	printf("a[5] = %d\n",*(*(ptr + 1) - 1));
	return 0;
}
