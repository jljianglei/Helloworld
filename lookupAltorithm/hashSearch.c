/*
 *哈希查找：哈希技术是存储位置和关键字之间确立的对应关系f，使得每个关键字key对应一个存储位置f(key),
 *查找时，根据这个对应关系找到f(key)。有可能对于key1 != key2但是f(key1) == f(key2)的情况，这个时候就是发生冲突了
 *，可以去解决冲突，找到合适的位置。
 *直接定址和冲突解决是哈希查找的2个特征
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define HASH_MAX_SIZE 20
#define NULL_KEY 0xffffffff
static int hashTable[HASH_MAX_SIZE];
void hashTableInit(int * hashTable,int length) {
     int i;
	 for(i = 0; i < length; ++i) {
	     hashTable[i] = 0xffffffff;/*位置为空的判断标准*/
	 }
}
/*hash函数*/
int hashMap(int data) {
   return data % HASH_MAX_SIZE;
}

void insertElem(int * hashTable,int data) {
   int hashAddr = hashMap(data);
   while(hashTable[hashAddr] != NULL_KEY) {
      hashAddr = (++hashAddr) % HASH_MAX_SIZE;
   }
   hashTable[hashAddr] = data;
}
int hashSearch(int * hashTable,int data) {
	if(hashTable == NULL)
		return;
    int hashAddr = hashMap(data);
	while(hashTable[hashAddr] != data) {
	   hashAddr = (++hashAddr) % HASH_MAX_SIZE;
	   if(hashTable[hashAddr] == NULL_KEY || hashAddr == hashMap(data))
		   return -1;
	}
	return hashAddr;
}
void printCommonTable(int * commonTable,int length) {
    printf("commonTable elem is :\n");
	int i;
	for(i = 0; i < length; ++i) {
	    printf("%d ",commonTable[i]);
	}
	printf("\n");
}
void printHashTable(int * hashTable,int length) {
    printf("hash table elem is :\n");
	int i;
	for(i = 0; i < length; ++i) {
	    printf("%d ",hashTable[i]);
	}
	printf("\n");
}
int main() {
    int a[] = {23,15,36,78,100,64,8,0,41};
	int length = sizeof(a)/sizeof(a[0]);
	printf("length is %d\n",length);
	printCommonTable(a,length);
	/*hashTable初始化*/
	hashTableInit(hashTable,HASH_MAX_SIZE);
    int i;
	for(i = 0; i < length; ++i) {
	    insertElem(hashTable,a[i]);
	}
	printHashTable(hashTable,HASH_MAX_SIZE);
	int ret;
	int key = 100;
	ret = hashSearch(hashTable,key);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find %d,index of hashTable is %d\n",key,ret);
	}
	key = 78;
	ret = hashSearch(hashTable,key);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find %d,index of hashTable is %d\n",key,ret);
	}
	key = 91;
	ret = hashSearch(hashTable,key);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find %d,index of hashTable is %d\n",key,ret);
	}
	key = 41;
	ret = hashSearch(hashTable,key);
	if(ret == -1) {
	   printf("no find %d\n",key);
	}
	else {
	   printf("find %d,index of hashTable is %d\n",key,ret);
	}
	return 0;
}
