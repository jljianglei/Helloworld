/*实现快速排序，然后把在这个过程中打印的log保存到文件中*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>
#define my_snprintf(str,size,fmt,...) snprintf(str,size,"%s::%s:%d,"fmt,__FILE__,__func__,__LINE__,##__VA_ARGS__)
#define BUF_SIZE 1024
char buf[BUF_SIZE];
FILE * fp = NULL;
void quickSort(int * a,int low,int high) {
    int first,last;
    first = low;
    last = high;
    if(low >= high)
        return;
    if(a == NULL) {
       printf("a is null\n");
       my_snprintf(buf,BUF_SIZE,"a is null\n");
       if(fp != NULL) {
         fwrite(buf,strlen(buf),1,fp);
       }
       return;
    }
    int key = *(a + first);
    while(first < last) {
      /*从后向前找比key小的，如果找到，则和first位置的值交换*/ 
        while(first < last && (a[last] >= key))
            last--;
        a[first] = a[last];
        /*从前向后找比key大的，如果找到，则和last交换*/
        while(first < last && (a[first] <= key))
            first++;
        a[last] = a[first];
    }
    a[first] = key;
    quickSort(a,low,first - 1);
    quickSort(a,first + 1,high);
}
void print(int * a,int length) {
   printf("the arr elem is:\n");
   my_snprintf(buf,BUF_SIZE,"the arr elem is:\n");
   if(fp){
       fwrite(buf,strlen(buf),1,fp);
   }
   int i;
   memset(buf,0,BUF_SIZE);
   for(i = 0; i < length; ++i) {
       printf("%d ",a[i]);
   }
   printf("\n");
}
int a[] = {20,4,18,0,10,30,45,6,100,0};
int main() {
    fp = fopen("log.txt","wb+");
    if(fp == NULL) {
       printf("fopen faild\n");
    }
    int length = sizeof(a)/sizeof(a[0]);
    print(a,length);
    quickSort(a,0,length - 1);
    print(a,length);
    fclose(fp);
    return 0;
}
