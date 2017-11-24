#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*
 *问题描述：把a，b2个数组交叉写到文件中，即先写a[0],再写b[0]
 *
 *
 *
 */
void printfFile(FILE* fp) {
     int * tmp = (int*)malloc(sizeof(int)*2);
	 while(!feof(fp)) {
	    fread(tmp,sizeof(int)*2,1,fp);
		printf("%d,%d ",*tmp,*(tmp + 1));
	 }
	 printf("\n");
}
int main() {
   FILE* fp = NULL;
   int a[5] = {1,2,3,4,5};
   int b[5] = {11,22,33,44,55};
   fp = fopen("test.txt","wb+");
   if(fp == NULL) {
      perror("open faild\n");
	  return 1;
   }
   int i;
   int length = sizeof(a)/sizeof(int);
   for(i = 0; i < length; ++i) {
      fwrite(a + i,sizeof(int),1,fp);
	  fwrite(b + i,sizeof(int),1,fp);
   }
   rewind(fp);
   printfFile(fp);
   fclose(fp);
   return 0;
}
