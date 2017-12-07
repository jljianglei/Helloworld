#include<stdio.h>
#include<stdlib.h>
int main() {
    char* p = malloc(10);
	free(p);
	p = NULL;
	free(p);
	return 0;
}
