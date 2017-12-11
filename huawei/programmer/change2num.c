#include<stdio.h>
void exchange(int * p,int * q) {
     if(p == NULL || q == NULL) {
	    printf("q or q is NULL\n");
	    return;
	 }
	 *p = (*p)^(*q);
	 *q = (*p)^(*q);
	 *p = (*p)^(*q);
}
void exchange2(int *p,int * q) {
     if(p == NULL || q == NULL) {
	    printf("p or q is NULL\n");
		return 0;
	 }
	 int tmp;
	 tmp = *p;
	 *p = *q;
	 *q = tmp;
}
void exchange3(int *p,int * q) {
     if(p == NULL || q == NULL) {
	    printf("p or q is NULL\n");
		return 0;
	 }
	 *p = *p + *q;/*a = a + b有可能超出整数表达的范围，所以用异或比较好*/
	 *q = *p - *q;
	 *p = *p - *q;
}
int main() {
    int a = 10;
	int b = 1;
	printf("a = %d,b = %d\n",a,b);
	exchange(&a,&b);
	printf("a = %d,b = %d\n",a,b);
	exchange(NULL,NULL);
	exchange2(&a,&b);
	printf("a = %d,b = %d\n",a,b);
	exchange3(&a,&b);
	printf("a = %d,b = %d\n",a,b);
    return 0;
}
