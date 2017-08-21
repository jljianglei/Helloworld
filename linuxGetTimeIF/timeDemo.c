#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
int main() {
    time_t t;
    time(&t);
    printf("%d\n",t);
    return 0;
}
