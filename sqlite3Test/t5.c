#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main() {
    char * str[] = {"dawei","geliya","damo"};
    printf("%s\n",str[0]);
    printf("%s,%s,%s\n",*str,*(str + 1),*(str + 2));
    return 0;
}
