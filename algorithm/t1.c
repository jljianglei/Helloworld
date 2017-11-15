#include<stdio.h>
/*
 *问题：
 *将数组中的所有元素依次循环右移k个位置 
 *
 *思路：以一个元素为起点，假设就是第一个元素，用pre_tmp记下其值
 *把它和右边距离为k的位置的元素交换，再移动到下一个k位，继续交换，直到回到原点
 *用一个变量记录交换的次数，如果等于数组长度，则交换结束，否则以当前元素的下一个位置为起点，
 *重新运算一次
 *
 */
void rightShift(int a[],int n,int k) {
     int p,q,pre_tmp,tmp;
	 int m = 0;
	 q = p = 0;
	 pre_tmp = a[p];
	 while(m < n) {
	    do {
			q = (q + k) % n;
			tmp = pre_tmp;
			pre_tmp = a[q];
			a[q] = tmp;
			m++;
		}while(q != p);
        if(m < n) {
		   q = ++p;
		   pre_tmp = a[p];
		}
	 }
}
int main() {
    int a[] = {1,2,3,4};
	int i;
	printf("before right shift\n");
	for(i = 0; i < sizeof(a)/sizeof(int); ++i) {
	    printf("%d ",a[i]);
	}
	printf("\n");
	rightShift(a,sizeof(a)/sizeof(int),1);
	printf("after right shift\n");
	for(i = 0; i < sizeof(a)/sizeof(int); ++i) {
	    printf("%d ",a[i]);
	}
	printf("\n");
	return 0;
}

