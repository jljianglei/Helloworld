#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef unsigned char uint8_t;
void countOf1(uint8_t a,int * num) {
     *num = 0;
	 int i;
	 for(i = 7; i >= 2; --i) {
	    if(a & (1 << i)) {
		   (*num)++;
		}
		else {
		   break;
		}
	 }
}
void formatUtf8(uint8_t * buf,int len) {
     int i;
	 int j;
	 int num;
	 printf("len = %d\n",len);
	 for(i = 0; i < len;) {
	    countOf1(buf[i],&num);
		printf("i = %d\n",i);
		printf("num = %d\n",num);
#if 0
		if((i + num - 1) >= len) {
		   for(j = i;j < len; j++) {
			  printf("buf[%d] = 0x00\n",j);
		      buf[j] = 0x00;
		   }
		}
		if(num == 0) {
		   ++i;
		}
		else {
		   i += num;
		}
#else
		if(num == 0) {
		  ++i;
		}else {
		  if((i + num - 1) >= len) {
		    for(j = i; j < len; ++j) {
			   printf("buf[%d] = 0x00\n",j);
		       buf[j] = 0x00;
			}
		  }
		  i += num;
		}
#endif
	 }
}
int main() {
    const unsigned char * str = "一一二二345";
	unsigned char buf[10 + 1] = {0};
	memcpy(buf,str,9);
	int i;
	printf("%s,len %d\n",str,strlen(str));
	for(i = 0; i < strlen(str); ++i) {
	   printf("%X ",str[i]);
	}
	printf("\n\n");
	printf("buf %s,len %d\n",buf,strlen(buf));
	for(i = 0; i < strlen(buf); ++i) {
	   printf("%X ",buf[i]);
	}
	printf("\n\n");
	formatUtf8(buf,strlen(buf));
	printf("buf %s,len %d\n",buf,strlen(buf));
	printf("\n\n");
	unsigned char c = 0xe4;
	printf("%x\n",c);
	return 0;
}
