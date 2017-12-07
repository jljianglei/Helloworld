#include<stdio.h>
int main() {
   const int a = 10;
   //a = 6;
   int arr[a];
   arr[a - 1] = 9;
   printf("%d\n",arr[a - 1]);
   printf("sizeof(arr) %d\n",sizeof(arr));
   int * p = (int*)&a;
   *p = 6;
   const char* str = "123";
   //*str = '1';
   char* q = (char*)str;
   //*q = '2';
   printf("a = %d\n",a);
   printf("%d\n",arr[a - 1]);
   printf("sizeof(arr) %d\n",sizeof(arr));
   return;
}
