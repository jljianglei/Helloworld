/*字符串的拷贝*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char* strcpyDemo(char * dst,const char * src) {
    if(dst == NULL || src == NULL)
		return NULL;
	char * tmp = dst;/*保存目的地址在临时变量中*/
	while((*dst++ = *src++) != '\0');
	return tmp;
}

int main() {
    char dst[100];
	const char * str = "strcpy";
	strcpyDemo(dst,str);
	printf("dst = %s\n",dst);
	return 0;
}
