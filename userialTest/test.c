#include "stc_userial.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
void * readLoop(void *msg) {
	int fd = *((int*)msg);
	uint16_t len = 10;
	uint8_t *buf = (uint8_t*)calloc(1,len);
	printf("fd = %d\n",fd);
	int ret = 0;
	while(1) {
	    ret = select_read(fd,buf,len - 1);
		printf("buf %s,ret %d\n",buf,ret);
		memset(buf,0,len);
		if(ret == -1)
			break;
	}
	printf("exit readloop\n");
	return NULL;
}
void * writeLoop(void *msg) {
	int num = 0;
	int fd = *((int*)msg);
	int ret = -1;
	while((num++) < 10) {
		uint16_t len = 10;
		uint8_t *buf = (uint8_t*)calloc(1,len);
		memcpy(buf,"12",len);
		ret = userial_write(fd,buf,strlen(buf));
	    printf("write %s,len %d\n",buf,ret);
	}
}
int main() {
	int fd = -1;
	fd = open("a.txt",O_RDWR,0x777);
	printf("fd = %d\n",fd);
	if(fd < 0) {
	   printf("open faild\n");
	   close(fd);
	   return -1;
	}
	pthread_t pid = -1,pid2;
#if 1
    if(pthread_create(&pid,NULL,readLoop,(void*)&fd) != 0) {
	   printf("pthread create faild\n");
	   return -1;
	}
#else
    if(pthread_create(&pid,NULL,writeLoop,(void*)&fd) != 0) {
	   printf("pthread create faild\n");
	   return -1;
	}
#endif
	usleep(1000);
	send_wakeup_signal(STC_USERIAL_RX_EXIT);
	sleep(1);
	return 0;
}
