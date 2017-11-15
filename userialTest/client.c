#include<stdio.h>  
#include<sys/types.h>  
#include<fcntl.h>  
#include<termios.h>  
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include "stc_userial.h"
#include<pthread.h>
#define BAUDRATE B38400  
#define MODEMDEVICE "/dev/ttyUSB0"  
void* readLoop(void *msg) {
	  int fd = *((int*)msg);
	  printf("fd = %d\n",fd);
	  char buf[256] = {0};
	  int len = 256;
      int ret = 0;
	  tcflush(fd,TCIOFLUSH);
	  while(1) {
		  ret = select_read(fd,buf,len);
		  printf("buf %s,ret %d\n",buf,ret);
		  if(ret == -1)
			  break;
	  }
}
int main()  
{  
	int fd,c = 0,res;  
	struct termios oldtio,newtio;
    uint16_t len = 256;	
	char buf[256] = {0}; 
	uint16_t ret = 0;
	int i;
	printf("start ...\n"); 
	pthread_t pid = -1;
	fd = open(MODEMDEVICE,O_RDWR | O_NDELAY);  
	if(fd < 0)  
	{  
		perror(MODEMDEVICE);  
		exit(1);  
	}  
	printf("open...,fd = %d\n",fd);  
	tcgetattr(fd,&oldtio); 
    printf("c_iflag %X,c_oflag %X,c_cflag %x,c_lflag %x\n",oldtio.c_iflag,oldtio.c_oflag,oldtio.c_cflag,oldtio.c_lflag);
	uint16_t ispeed = cfgetispeed(&oldtio);
	uint16_t ospeed = cfgetospeed(&oldtio);
	printf("ispeed %d,ospeed %d\n",ispeed,ospeed);
	for(i = 0; i < NCCS; ++i) {
	   printf("%X ",oldtio.c_cc[i]);
	}
	printf("\n");
	bzero(&newtio,sizeof(newtio));  
#if 0
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL;  
	newtio.c_iflag = IGNPAR;  
	newtio.c_oflag = 0;  
	newtio.c_lflag = ICANON;  
	tcflush(fd,TCIFLUSH);  
	tcsetattr(fd,TCSANOW,&newtio);  
#endif
	printf("reading...\n");  
#if 0
	res = read(fd,buf,255);  
	buf[res] = 0;  
	printf("res=%d vuf=%s\n",res,buf);  
	if(buf[0]=='a') break;  
#endif
	pthread_create(&pid,NULL,readLoop,(int*)&fd);
	sleep(2);
	send_wakeup_signal(STC_USERIAL_RX_EXIT);
	sleep(1);
	printf("close...\n");  
	//tcsetattr(fd,TCSANOW,&oldtio);  
	close(fd);  
	return 0;  
}
