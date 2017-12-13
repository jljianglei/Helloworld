/*指针使用*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef void (*Cb)(int a,int b);
static Cb localcb;
void registerCb(Cb arg) {
    localcb = arg;
}
void funcCb(int a,int b) {
     printf("a = %d,b = %d\n",a,b);
}
void func(int b[][3]) {
    printf("sizeof(b) = %d\n",sizeof(b));
	int (*q)[3] = b;
	int i = 0; 
	for(i = 0; i < 3; ++i) {
	    printf("%d ",*(*q + i));
	}
	printf("\n");
}
int main() {
    int a[] = {1,2,3,4};
	int b[][3] = {{1,10,100},{2,23,100},{3,4,5}};
	printf("sizeof(a) = %d,sizeof(b) = %d\n",sizeof(a),sizeof(b));
	int * p = a;
	printf("a[3] = %d,%d,%d\n",a[3],*(a + 3),*(p + 3));
	int (*q)[3] = b;
	printf("b[2][2] = %d,%d\n",b[2][2],*(*(q + 2) + 2));
	func(b);
	void (*funcP)(int b[][3]) = &func;
	(*funcP)(b);
	int i = 10;
	int j = 11;
	registerCb(&funcCb);
	if(localcb != NULL) {
	   (*localcb)(i,j);
	}
	return 0;
}
