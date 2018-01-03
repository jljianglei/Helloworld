#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int get(int m,int n) {
    if(m == 0 || n == 0)
        return 0;
    while(n != 0) {
        int tmp = m % n;
        m = n;
        n = tmp;
    }
    return m;
}
int main() {
    int M,N;
    printf("input M,N\n");
    scanf("%d%d",&M,&N);
    int ret = get(M,N);
    printf("%d,%d最大公约数是%d\n",M,N,ret);
}
