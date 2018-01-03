#include<stdio.h>
static int b;
void func() {
  printf("--------------------\n"); 
}

static void func2() {
  printf("-------------------------\n");
}
int main() {
    int a = 10;
	int arr[] = {1,1,1,1,1,1,11};
	static int c = 11;
	const char * str = "helloworld";
	int * p = malloc(sizeof(int));
	printf("&a = %p\n",&a);
	printf("global &b = %p\n",&b);
	printf("local static &c = %p\n",&c);
	printf("const &str = %p\n",str);
	printf("malloc p %p\n",p);
	printf("func addr %p\n",&func);
	printf("func2 addr %p\n",&func2);
	printf("arr addr %p\n",arr);
	return 0;
}
