#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/mman.h>
typedef struct{
    int index;
    char text[10];
}Record;
#define SIZE 10
#define EDIT_INDEX 5
void printFile(FILE* fp) {
   Record record;
   int ret;
   while((ret = fread(&record,sizeof(record),1,fp)) == 1) {
         printf("%2d,%10s\n",record.index,record.text);
   }
   printf("\n");
}
int main() {
    Record record;
    Record* p_mmap_addr;
    FILE* fp = fopen("record.txt","rb+");
    if(fp == NULL) {
       printf("fopen faild\n");
       return -1;
    }
    printf("fp = %p\n",fp);
    printFile(fp);
    printf("fp = %p\n",fp);
    printf("line %d\n",__LINE__);
#if 0
    FILE* fp = fopen("record.txt","wb+");
    if(fp == NULL) {
       printf("fopen faild\n");
       return -1;
    }
    int i;
    for(i = 0; i < SIZE; ++i) {
        record.index = i;
        snprintf(record.text,9,"No.%d",i);
        record.text[9] = '\0';
        fwrite(&record,sizeof(Record),1,fp);
    }
    /*重定向文件指针到文件开头*/
    fseek(fp,0,SEEK_SET);
    printFile(fp);
#endif
    /*修改第6条记录的index为index * 10*/
#ifndef USE_MMAP
    fseek(fp,sizeof(Record) * EDIT_INDEX,SEEK_SET);
    fread(&record,sizeof(record),1,fp);
    record.index *= 10;
    snprintf(record.text,9,"No.%d",record.index);
    record.text[9] = '\0';
    /*write*/
    fseek(fp,sizeof(Record)*EDIT_INDEX,SEEK_SET);
    fwrite(&record,sizeof(record),1,fp);
    fseek(fp,0,SEEK_SET);
    printFile(fp);
#else
    int fd = open("record.txt",O_RDWR);
    if(fd < 0) {
       perror("open faild");
       return -1;
    }
    p_mmap_addr = mmap(NULL,SIZE*sizeof(Record),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(p_mmap_addr == MAP_FAILED) {
       perror("mman faild");
       return -1;
    }
    (p_mmap_addr + EDIT_INDEX)->index = EDIT_INDEX * 10;
    snprintf((p_mmap_addr + EDIT_INDEX)->text,9,"No.%d",(p_mmap_addr + EDIT_INDEX)->index);
    (p_mmap_addr + EDIT_INDEX)->text[9] = '\0';
    msync(p_mmap_addr,SIZE*sizeof(Record),MS_SYNC);
    /*释放*/
    munmap(p_mmap_addr,SIZE*sizeof(Record));
    printf("fp = %p\n",fp);
    printf("line %d\n",__LINE__);
    fseek(fp,0,SEEK_SET);
    printf("fp = %p\n",fp);
    printFile(fp);
#endif/*USE_MMAP*/
    fclose(fp);
    return 0;
}
