#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc,char** argv) {
	char passwd[10];
	int flag = 0;
	printf("flag addr %p,passwd addr %p\n",&flag,passwd);
	if(argc < 2) {
	   printf("argc must == 2\n");
	   return 1;
	}
	strcpy(passwd,argv[1]);
	if(strcmp("linux",passwd) == 0) {
	   flag = 1;
	}
	if(flag) {
	   printf("passwd correct\n");
	}
	else {
	   printf("passwd error\n");
	}
	return 0;
}
