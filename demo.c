#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
int main() {
    struct timeval t1,t2;
    gettimeofday(&t1,NULL);
    printf("t1.tv_sec %ld,t1.tv_usec %ld\n",t1.tv_sec,t1.tv_usec);
    usleep(2000000);
    gettimeofday(&t2,NULL);
    printf("t2.tv_sec %ld,t2.tv_usec %ld\n",t2.tv_sec,t2.tv_usec);
    long timesub = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
    printf("timesub %ld(ms)\n",timesub);
    return 0;
}
