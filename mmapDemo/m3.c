#include<unistd.h>
#include<stdio.h>
int main() {
    printf("getpagesize = %d\n",getpagesize());
    printf("size of a page in bytes:%ld\n",sysconf(_SC_PAGESIZE));
    printf("max length of hostname %ld\n",sysconf(_SC_HOST_NAME_MAX));
    printf("the num of clock ticks per second:%ld\n",sysconf(_SC_CLK_TCK));
    return 0;
}
