#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
int main() {
    time_t t;
    struct tm * p;
    time(&t);
    printf("%d(s)\n",t);
    p = localtime(&t);
    printf("%d(year)-%d(month)-%d(day),%d(hour)-%d(minute)-%d(sec)\n",p->tm_year + 1900,p->tm_mon + 1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    char *str = asctime(p);
    printf("%s\n",str);
    return 0;
}
