#include<stdio.h>
int my_strcmp(char* src1,char * src2) {
    int result = 1;
	if(src1 == NULL || (src2 == NULL))
		return result;
	while((*src1 == *src2) && (*src1 != '\0' && *src2 != '\0')) {
	    src1++;
		src2++;
	}
	if((*src1 == '\0' && *src2 == '\0')) {
	   result = 0;
	}
	return result;
}
int main() {
    char * s1 = "123";
	char * s2 = "1234";
	if(my_strcmp(s1,s2) == 0) {
	   printf("%s == %s\n",s1,s2);
	}
	else {
	   printf("%s != %s\n",s1,s2);
	}
	return 0;
}
