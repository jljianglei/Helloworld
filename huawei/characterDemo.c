/*
 *第一个只出现一次的字符
 *
 *
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define HASH_TABLE_LENGTH 256
void findFirstAppearOnlyOne(const char * str) {
     if(str == NULL)
		 return;
	 int length = strlen(str);
	 printf("length = %d\n",length);
	 int i;
	 int j;
	 for(i = 0; i < length; ++i) {
	    for(j = i + 1; j < length; ++j) {
		    if(str[i] == str[j])
				break;
		}
		if(j >= length) {
		   printf("find it,the character is %c\n",str[i]);
		   break;
		}
	 }
}


void findFirstAppearOnlyOne2(const char* str) {
    if(str == NULL)
		return;
	unsigned int hashTable[HASH_TABLE_LENGTH] = {0};
	char * p = str;
	while(*p != '\0') {
	   printf("*p = %d\n",*p);
	   hashTable[*(p++)]++;
	}
	printf("--------------------------\n");
	p = str;
	while(*p != '\0') {
	   if(hashTable[*p] == 1) {
		  printf("*p = %d,hashTable[*p] = %d\n",*p,hashTable[*p]);
	      printf("find it ,the character is %c\n",*p);
		  return;
	   }
	   p++;
	}
}
int main() {
    char * str = "abacde";
	findFirstAppearOnlyOne(str);
	findFirstAppearOnlyOne2(str);
	return 0;
}

