#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main() {
    unsigned int a = 6;
	int b = -20;
	printf("a + b = %u\n",a + b);
	if(a + b > 6) {
	   printf("> 6\n");
	}
	else {
	   printf("< 6\n");
	}
	return 0;
}
