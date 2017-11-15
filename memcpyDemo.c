#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char* strncpyDemo(char* dst,const char* src,int n) {
     int i;
	 for(i = 0; i < (n - 1) && (src[i] != '\0'); ++i)
		 dst[i] = src[i];
	 dst[i] = '\0';
	 return dst;
}
char* memcpyDemo(char * dst,const char* src,int len) {
     int i;
	 for(i = 0; i < len; ++i)
		 dst[i] = src[i];
	 return dst;
}
int main() {
    char dst[20] = {0};
	int i;
	for(i = 0; i < 20; ++i) {
        printf("%02X ",dst[i]);
	}
	printf("\n");
	const char * src = "helloworld";
	for(i = 0; i< 20; ++i) {
	    printf("%02X ",src[i]);
	}
	printf("\n");
	memcpyDemo(dst,src,20);
	for(i = 0; i < 20; ++i) {
        printf("%02X ",dst[i]);
	}
	printf("\n");
	printf("%s,len %d\n",dst,strlen(dst));
	return 0;
}
