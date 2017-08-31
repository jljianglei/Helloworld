#include<stdio.h>  
#include<sys/types.h>  
#include<fcntl.h>  
#include<termios.h>  
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#define BAUDRATE B38400  
#define MODEMDEVICE "/dev/ttyS0"  
int main()  
{  
	int fd,c = 0,res;  
	struct termios oldtio,newtio;  
	char buf[256];  
	printf("start ...\n");  
	fd = open(MODEMDEVICE,O_RDWR | O_NOCTTY);  
	if(fd < 0)  
	{  
		perror(MODEMDEVICE);  
		exit(1);  
	}  
	printf("open...,fd = %d\n",fd);  
	tcgetattr(fd,&oldtio);  
	bzero(&newtio,sizeof(newtio));  
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;  
	newtio.c_iflag = IGNPAR;  
	newtio.c_oflag = 0;  
	newtio.c_lflag = ICANON;  
	tcflush(fd,TCIFLUSH);  
	tcsetattr(fd,TCSANOW,&newtio);  
	printf("reading...\n");  
	while(1)  
	{  
		printf("into while(1)\n");
		res = read(fd,buf,255);  
		buf[res] = 0;  
		printf("res=%d vuf=%s\n",res,buf);  
		if(buf[0]=='a') break;  
	}  
	printf("close...\n");  
	close(fd);  
	tcsetattr(fd,TCSANOW,&oldtio);  
	return 0;  
}
