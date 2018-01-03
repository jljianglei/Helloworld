#include<stdio.h>
#define CONFIG_CRIS
#ifdef CONFIG_CRIS 
#warning "the CRIS is defined"
#endif
#error "this is a error"
int main() {
    printf("hello,world\n");
    return 0;
}
