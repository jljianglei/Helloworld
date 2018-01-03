#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include "common.h"
int main() {
    int fd = open("message.txt",O_RDWR | O_CREAT,0775);
    if(fd < 0) {
       perror("open faild");
       return -1;
    }
    Message* p_msg = mmap(NULL,sizeof(Message),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(p_msg == MAP_FAILED) {
       perror("mmap faild");
       return -1;
    }
    while(1) {
       printf("read msgid is %d\n",p_msg->id);
       sleep(1);
    }
    return 0;
}
