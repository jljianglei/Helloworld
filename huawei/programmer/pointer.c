#include<stdio.h>
struct A {
   int a;
   char * p;
   char c;
   int b;
};
int main() {
    printf("sizeof(int*) = %d\n",sizeof(int*));
    printf("sizeof(void*) = %d\n",sizeof(void*));
	printf("sizeof(int) = %d,sizeof(char) = %d,sizeof(char*) = %d\n",sizeof(int),sizeof(char),sizeof(char*));
	printf("sizeof(A) = %d\n",sizeof(struct A));
	int * p = (int*)0x8000;
	*p = 0xaabb;
	return 0;
}
