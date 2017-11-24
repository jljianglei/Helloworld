#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define CAPTURE_PATH "./test_record"
void printFile(FILE* fp) {
   int length = 0;
   int ret = 0;
   ret = fread(&length,1,sizeof(int),fp);
   if(ret != sizeof(int)) {
      printf("fread faild\n");
	  return;
   }
   printf("length %d\n",length);
   int *tmp = (int*)malloc(length*sizeof(int));
   fread(tmp,1,length*sizeof(int),fp);
   int i;
   for(i = 0; i < length; ++i) {
       printf("%d ",*(tmp + i));
   }
   printf("\n");
   free(tmp);
   ret = fread(&length,1,sizeof(int),fp);
   printf("length %d\n",length);
   tmp = (int*)malloc(length*sizeof(int));
   fread(tmp,1,length*sizeof(int),fp);
   for(i = 0; i < length; ++i) {
       printf("%d ",*(tmp + i));
   }
   printf("\n");
   free(tmp);
}
int main() {
   FILE* fp = NULL;
   fp = fopen(CAPTURE_PATH,"wb+");
   if(fp == NULL) {
      perror("open faild");
	  return 1;
   }
   int a[] = {1,2,3,4,5};
   int b[] = {11,12,13,14,1,1,1,1,1};
   int c[] = {111,112,113,114,115,116};
   int ret = 0;
   int length = sizeof(a)/sizeof(int);
   ret = fwrite(&length,1,sizeof(int),fp);
   if(ret != sizeof(int)) {
      printf("fwrite faild\n");
	  return 1;
   }
   ret = fwrite(a,1,sizeof(a),fp);
   if(ret != sizeof(a)) {
      printf("fwrite faild\n");
	  return 1;
   }
   length = sizeof(b)/sizeof(int);
   ret = fwrite(&length,1,sizeof(int),fp);
   if(ret != sizeof(int)) {
      printf("fwrite faild");
	  return 1;
   }
   ret = fwrite(b,1,sizeof(b),fp);
   if(ret != sizeof(b)) {
      printf("fwrite faild\n");
	  return 1;
   }
   length = sizeof(c)/sizeof(int);
   ret = fwrite(&length,1,sizeof(int),fp);
   if(ret != sizeof(int)) {
      printf("fwrite faild");
	  return 1;
   }
   ret = fwrite(b,1,sizeof(c),fp);
   if(ret != sizeof(c)) {
      printf("fwrite faild\n");
	  return 1;
   }
   rewind(fp);
   printFile(fp);
   close(fp);
   return 0;
}
