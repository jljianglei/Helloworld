#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void printFile(FILE* fp) {
   int * tmp = (int*)malloc(sizeof(int)*2);
   while(!feof(fp)) {
      fread(tmp,sizeof(int)*2,1,fp);
	  printf("%d,%d ",*tmp,*(tmp + 1));
   }
   printf("\n");
   if(tmp) {
      free(tmp);
   }
}
int main(int argc,char** argv) {
	if(argc != 2) {
	   printf("usage ./a.out <filename>\n");
	   return 1;
	}
	FILE* fp = NULL;
	FILE* newFile = NULL;
	fp = fopen(argv[1],"rb+");
	if(fp == NULL) {
	   perror("open faild\n");
	   return 1;
	}
	newFile = fopen("newFile.txt","wb+");
	if(newFile == NULL) {
	  perror("open faild\n");
	  return 1;
	}
	int * tmp = (int*)malloc(sizeof(int)*2);
	while(!feof(fp)) {
	  fread(tmp,sizeof(int)*2,1,fp);
	  int tmpa = *(tmp);
	  int tmpb = *(tmp + 1);
	  tmpb /= 2;
	  fwrite(&tmpa,sizeof(int),1,newFile);
	  fwrite(&tmpb,sizeof(int),1,newFile);
	}
	rewind(fp);
	rewind(newFile);
	printFile(fp);
	printFile(newFile);
	fclose(fp);
	fclose(newFile);
    return 0;
}
