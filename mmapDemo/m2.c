/*通过内存读写向文件中写入结构体数组*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#define my_print(fmt,arg...) printf("%s,%d,"fmt,__func__,__LINE__,##arg)
typedef struct {
    int id;
    char text[10];
}Record;
static Record record[] = {{1,"No1"},{2,"No2"},{3,"No3"},{4,"No4"}};
int main() {
    int fd; 
    fd = open("record.txt",O_RDWR | O_CREAT,0775);
    if(fd < 0) {
       perror("open faild");
       return -1;
    }
    Record* ptr;
    ptr = mmap(NULL,sizeof(record),PROT_WRITE | PROT_READ,MAP_SHARED,fd,0);
    if(ptr == MAP_FAILED) {
      perror("mmap faild");
      return -1;
    }
    int i;
    int length = sizeof(record)/sizeof(record[0]);
    /*这里就是用内存读写替代了read/write操作*/
    for(i = 0; i < length; ++i) {
        (ptr + i)->id = record[i].id;
        memcpy((ptr + i)->text,record[i].text,10);
    }
    munmap(ptr,sizeof(record));
    close(fd);
    FILE * fp = fopen("record.txt","rb+");
    if(fp == NULL) {
       my_print("fopen faild\n");
       return -1;
    }
    int ret = 0;
    Record r1;
    int count = 0;
    while((ret = fread(&r1,sizeof(Record),1,fp)) == 1 && (count++ != 4)) {
        printf("%d,%s\n",r1.id,r1.text);
    }
    fclose(fp);
    return 0;
}
