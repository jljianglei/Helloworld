#include<stdio.h>
int main() {
    int * p = malloc(sizeof(int));
	*p = 10;
	printf("*p = %d\n",*p);
	free(p);
	printf("p = %p\n",p);
	*p = 1;
	printf("*p = %d\n",*p);
	return 0;
}

