#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "common.h"
int main() {
    int fd = open("message.txt",O_RDWR | O_CREAT,0775);
    if(fd < 0) {
       perror("open faild");
       return -1;
    }
    Message msg;
    memset(&msg,0,sizeof(Message));
    msg.id = MSG_0;
    int ret = write(fd,&msg,sizeof(msg));
    if(ret != sizeof(msg)) {
       perror("write faild");
       return -1;
    }
    Message* p_msg = mmap(NULL,sizeof(msg),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(p_msg == MAP_FAILED) {
       perror("mmap faild");
       return -1;
    }
    while(1) {
       printf("write msgId,current id is %d\n",p_msg->id);
       p_msg->id++;
       if(p_msg->id == MSG_MAX)
           p_msg->id = MSG_0;
       sleep(1);
    }
    return 0;
}
