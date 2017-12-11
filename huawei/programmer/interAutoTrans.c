#include<stdio.h>
int main() {
    unsigned int a = 6;
	int b = -20;
	char c;
	(a + b > 6)? (c = 1):(c = 0);/*c语言整数自动转换原则，当表达式中有符号类型和无符号类型时，所有操作数都自动转换为无符号的*/
	printf("c = %d\n",c);
	return 0;
}
