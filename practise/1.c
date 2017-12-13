#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define BIT_MASK(bits) (0x01 << (bits - 1))

#define print(var) printf("0x%x\n",var)
#define CONFIG_CRC

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


#define CRC1
#define CRC2
struct test {
   enum msgId {
	   MSG_1,
	   MSG_2,
	   MSG_3,
	   MSG_4,
	   MSG_MAX
   }mMsgId;
   struct msg1 {
       int a1;
   };
   struct msg2 {
       int a2;
   };
   struct msg3 {
       int a3;
   };
   struct msg4 {
       int a4;
   };
   union {
	   struct msg1 m1;
	   struct msg2 m2;
	   struct msg3 m3;
	   struct msg4 m4;
   }u1;
};
void arrayHandler(int * a,int length) {
    int i;
	int tmp = *a;
	for(i = 0; i < length; ++i) {
	    *(a + i) = *(a + i) /tmp;
	}
}
void reverseArray(int * a,int length) {
    int low = 0;
	int high = length - 1;
	int tmp;
	while(low <= high) {
	   tmp = a[low];
	   a[low] = a[high];
	   a[high] = tmp;
	   low++;
	   high--;
	}
}
void printArray(int * a,int length) {
    int i;
	for(i = 0; i < length; ++i) {
	    printf("%d ",*(a + i));
	}
	printf("\n");
}
void print2Array(int (*a)[3],int size) {
     int i,j;
	 for(i = 0; i < size; ++i) {
	     for(j = 0; j < 3; j++) {
		     printf("%d ",*(*(a + i) + j));
		 }
		 printf("\n");
	 }
}
int arraySum(int a[],int length) {
    int result = 0;
	int i;
	for(i = 0; i < length; ++i) {
	   result += *(a + i);
	}
	return result;
}
int main() {
    uint8 a = 0xff;
	print(a);
    a &= ~BIT_MASK(2);
	print(a);
#if defined(CONFIG_CRC)
	printf("CONFIG_CRC\n");
#endif
    uint16 aa = 0;
	uint8 a1 = 0xf1;
	uint8 a2 =0xf4;
	aa = (a1 << 8) | a2;
	print(aa);
	uint8 * p = (uint8*)&aa;
	uint8 tmp;
	tmp = *p;
	*p = *(p + 1);
    *(p + 1) = tmp;
	print(aa);
	uint16 b = 0xec1d;
	uint8 b1 = (b >> 8 & 0xff);
	uint8 b2 = b & 0xff;
	print(b);
	print(b1);
	print(b2);
	struct test t1;
	t1.mMsgId = MSG_1;
	t1.u1.m1.a1 = 10;
	if(t1.mMsgId == MSG_1) {
	   t1.u1.m1.a1 = 100;
	}
	printf("%d\n",t1.u1.m1.a1);
#if defined(CRC1) && defined(CRC2)
	const char * str = "CRC1 & CRC2";
#endif
	int k[] = {2,10,23,50,34,78};
    int length = sizeof(k)/sizeof(k[0]);
	int result = arraySum(k,length);
	printf("result = %d\n",result);
	printArray(k,length);
	arrayHandler(k,length);
	printArray(k,length);
	reverseArray(k,length);
	printArray(k,length);
	int l[2][3] = {{1,2,3},{4,5,6}};
	print2Array(l,2);
	return 0;
}
