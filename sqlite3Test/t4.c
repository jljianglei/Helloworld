#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main() {
    const char * const str = "hello";
    printf("%s\n",str);
    char const *  ptr = "we";
    printf("%s\n",ptr);
    ptr = "omg";
    printf("%s\n",ptr);
    return 0;
}
