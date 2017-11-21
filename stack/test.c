#include<stdio.h>
int main() {
    char a[8192*1024];
	printf("a %p\n",a);
//	printf("a %p,a + 8192*1024 %p\n",a,a + 8192*2014);
	return 0;
}
