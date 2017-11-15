#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<termios.h>
#include<fcntl.h>
#include "stc_userial.h"
#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyUSB0"
int main() {
    int fd = -1;
	int c,res;
	struct termios oldtio,newtio;
	char ch,s1[20];
	char buf[256] = {0};
	int i;
	printf("start\n");
	fd = open(MODEMDEVICE,O_RDWR);
	if(fd < 0) {
      printf("open faild");	
	  return -1;
	}
//	lockf(fd,F_LOCK,0);
	printf("open,fd = %d\n",fd);
	tcgetattr(fd,&oldtio);
	//
	cfmakeraw(&oldtio);
	//use standard model
	//oldtio.c_lflag = ICANON;
    cfsetospeed(&oldtio,B9600);
    cfsetispeed(&oldtio,B9600);
	tcsetattr(fd,TCSANOW,&oldtio);
	tcflush(fd, TCIOFLUSH);
	printf("c_iflag %X,c_oflag %X,c_cflag %x,c_lflag %x\n",oldtio.c_iflag,oldtio.c_oflag,oldtio.c_cflag,oldtio.c_lflag);
#if 0
	oldtio.c_cflag &= ~CSIZE;
	oldtio.c_cflag |= CS8;
	oldtio.c_cflag &= ~CRTSCTS;
	oldtio.c_cflag &= ~PARENB;
	oldtio.c_oflag = 4;
	tcsetattr(fd,TCSANOW,&oldtio);
	tcflush(fd,TCIFLUSH);
    cfsetospeed(&oldtio,B9600);
    cfsetispeed(&oldtio,B9600);
	tcsetattr(fd,TCSANOW,&oldtio);
#endif
	uint16_t ispeed = cfgetispeed(&oldtio);
	uint16_t ospeed = cfgetospeed(&oldtio);
	printf("ispeed %d,ospeed %d\n",ispeed,ospeed);
	for(i = 0; i < NCCS; ++i) {
	   printf("%X ",oldtio.c_cc[i]);
	}
	printf("\n");
#if 0
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;
	tcflush(fd,TCIFLUSH); 
	tcsetattr(fd,TCSANOW,&newtio);
#endif
	printf("writing...\n");
	uint16_t len = 10;
	const char* str = "123456";
	uint8_t* buf2 = (uint8_t*)calloc(1,len);
	memcpy(buf2,str,strlen(str));
	while(1)  
	{  
#if 0
		while((ch=getchar())!='@')  
		{  
			s1[0]=ch;  
			res=write(fd,s1,1);  
		}  
		s1[0]=ch;  
		s1[1]='\n';  
		res=write(fd,s1,2);  
		break; 
#endif
		int total = userial_write(fd,buf2,len);
		printf("write total %d\n",total);
		usleep(1000000);
	}
	//printf("wait read\n");
    //res = read(fd,buf,255);
	//buf[res] = '\0';
	//printf("recv %s,len %d\n",buf,strlen(buf));
	printf("close...\n"); 
//	lockf(fd,F_ULOCK,0);
	close(fd);  
	return 0;  
}
