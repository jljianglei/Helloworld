#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int strcmpTest(const char * s1,const char * s2) {
    if(s1 == NULL || s2 == NULL)
		return -1;
	while(*s1 && (*s2)) {
	   if(*s1 != (*s2))
		   return *s1 - (*s2);
	   s1++;
	   s2++;
	}
	return *s1 - (*s2);
}
int strncmpTest(const char* s1,const char* s2,int n) {
    if(s1 == NULL || s2 == NULL)
		return -1;
	int i = 0;
	while(*s1 && (*s2) && (i < n) && (*s1 == *s2)) {
		  if(i == (n - 1))
			  return 0;
	      s1++;
		  s2++;
		  i++;
	}
	return *s1 - *s2;
}
int main() {
    const char * s1 = "abc";
	const char * s2 = "abcd";
	int ret = 0;
	ret = strcmpTest(s1,s2);
	if(ret == 0) {
	   printf("%s == %s\n",s1,s2);
	}
	else {
	   printf("%s != %s\n",s1,s2);
	}
	int length = 4;
	ret = strncmpTest(s1,s2,length);
	if(ret == 0) {
	   printf("%d of bytes %s == %s\n",length,s1,s2);
	}
	else {
	   printf("%d of bytes %s != %s\n",length,s1,s2);
	}
	return 0;
}
