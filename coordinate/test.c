#include<stdio.h>
int main() {
    double y = 400.0;
	double baseY = 800;
	double realBase = 861;
	double real = 861 * (y/baseY);
	printf("real %.2f\n",real);
	int realInt = (int)real;
	printf("realInt %d\n",realInt);
	printf("400/800  = %.2f\n",(double)400/800);
	if(0.6 > 0.5) {
	  printf("0.6 > 0.5\n");
	}
	return 0;
}
