#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<termios.h>
#include<fcntl.h>
#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyUSB0"
int main() {
    int fd = -1;
	int c,res;
	struct termios oldtio,newtio;
	char ch,s1[20];
	char buf[256] = {0};
	printf("start\n");
	fd = open(MODEMDEVICE,O_RDWR | O_NOCTTY);
	if(fd < 0) {
      printf("open faild");	
	  return -1;
	}
	printf("open,fd = %d\n",fd);
	tcgetattr(fd,&oldtio);
	bzero(&newtio,sizeof(newtio));
	newtio.c_cflag = BAUDRATE|CS8|CLOCAL|CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;
	tcflush(fd,TCIFLUSH); 
	tcsetattr(fd,TCSANOW,&newtio);
	printf("writing...\n");
	while(1)  
	{  
		while((ch=getchar())!='@')  
		{  
			s1[0]=ch;  
			res=write(fd,s1,1);  
		}  
		s1[0]=ch;  
		s1[1]='\n';  
		res=write(fd,s1,2);  
		break;  
	}
	sleep(1);
	printf("wait read\n");
    res = read(fd,buf,255);
	buf[res] = '\0';
	printf("recv %s,len %d\n",buf,strlen(buf));
	printf("close...\n");  
	tcsetattr(fd,TCSANOW,&oldtio);  
	close(fd);  
	return 0;  
}
