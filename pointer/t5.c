#include<stdio.h>
#define NDEBUG
#include<assert.h>
int main() {
    int * p = malloc(sizeof(int));
	assert(p == 0);
	*p = 1;
	return 0;
}
