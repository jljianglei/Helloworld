/*将字符串'askdaskaskdaskg'删除指定字符ask,删除后的结果是ddg*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main() {
    char buf[20];
	int i = 0;
	char* p,*sub = "ask";
	char* str = "askdaskaskdaskg",*str2;
	p = sub;
	str2 = str;
	while(*str2) {
	     while(*p) {
		    if(*p == *str2)
				break;
			p++;
		 }
		 if(*p == '\0')
			 buf[i++] = *str2;
		 p = sub;
		 str2++;
	}
	buf[i] = '\0';
	printf("%s\n",buf);
	return 0;
}
