#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main() {
   char * str = "/var/***********";/*字符串常量，不能修改，所以strcpy是非法的*/
   char a[10] = "/var/";/*数组是在栈区分配的，内容可以修改*/
   strcpy(a + 5,"abc");
   //strcpy(str + 5,"abc");
   printf("%s\n",a);
   return 0;
}

