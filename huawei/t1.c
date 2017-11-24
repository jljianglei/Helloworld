#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main() {
   char* str = "0xef12a010";
   printf("str %s\n",str);
   long addr = strtol(str,NULL,16);
   printf("addr 0x%x\n",addr);
   char * str2 = "1020";
   long num = strtol(str2,NULL,10);
   printf("num = %ld\n",num);
   return 0;
}
