#include<string.h>
#include<stdio.h>
int main() {
    const char * str = "helloworld";
    printf("%s\n",str);
    char * p;
    p = strchr(str,'w');
    printf("%s\n",p);
    return 0;
}
