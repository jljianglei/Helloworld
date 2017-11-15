/*
 *c程序的内存分配，c的内存分配包括代码区，数据区，代码区，顾名思义存放代码的，数据区可以细分为
 *堆区，栈区（或者叫堆栈）,全局区（或叫静态数据区），常量区（字符串常量等）
 *
 *
 */

#include<stdio.h>
extern int c;
int c = 0;
int m = 10;
const static int k = 10;
void func(int arg) {
    static int a = 10;
	printf("static a addr %p\n",&a);
	int b;
	printf("local b addr %p\n",&b);
	int *p = malloc(10);
	printf("int *p addr %p\n",p);
	if(p != NULL)
		free(p);
}
int main() {
    const int a = 10;
	char * str = "123";
	printf("const a addr %p\n",&a);
	printf("const str addr %p\n",str);
	printf("global m addr %p\n",&m);
	printf("static global k addr %p\n",&k);
	func(0);
	return 0;
}
