#include<stdio.h>
int sumFac(int n) {
	if(n <= 0)
		return 0;
	else 
	if(n == 1 || (n == 2))
		return 1;
	else 
		return sumFac(n - 1) + sumFac(n - 2);
}
/*1,1,2,3,5,8,13,21,34*/
int sumFac2(int n) {
    int sum = 0;
	int f1 = 1;
	int f2 = 1;
	if(n <= 0) {
	   return 0;
	} else if(n == 1 || (n == 2)) {
	   return 1;
	}
	else {
		int i;
		for(i = 2; i < n; ++i) {
		    sum = f1 + f2;
			f1 = f2;
			f2 = sum;
		}
		return sum;
	}
}
int main() {
    int n = 1;
	printf("1,1,2,3,5,8,13,21,34,55\n");
	printf("sumFac(1) %d\n",sumFac(n));
	n = 2;
	printf("sumFac(2) %d\n",sumFac(n));
	n = -1;
	printf("sumFac(-1) %d\n",sumFac(n));
	n = 5;
	printf("sumFac(5) %d\n",sumFac(n));
	n = 8;
	printf("sumFac(8) %d\n",sumFac(n));
	printf("sumFac(8) %d\n",sumFac2(n));
	return 0;
}
