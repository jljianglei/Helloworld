#include<stdio.h>
int main() {
    void * p;
	int a = 10;
	p = (void*)&a;
	printf("&a = %p,p = %p\n",&a,p);
	printf("a = %d,*p = %d\n",a,*(int*)p);
	p++;
	printf("after +1,p = %p\n",p);
	void * q = "123";
	q++;
	printf("%s\n",q);
	return 0;
}
