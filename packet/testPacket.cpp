#include "stc_com_coder.h"
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<termio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#define COM_PATH "/dev/ttyACM0"
using namespace std;
int main() {
	DataFrame frame;
	frame.ct = 0x5A;
	printf("before encode\n");
	printf("%02X ",frame.ct);
	for(int i = 0; i < ST_DATA_LEN; ++i) {
	   frame.stData[i] = i;
       printf("%02X ",frame.stData[i]);
	}
	frame.dataSize = 320;
	frame.data = (uint8_t*)calloc(1,frame.dataSize);
	frame.data[0] = 0x5A;
	frame.data[1] = 0x02;
	frame.data[180] = 0x5A;
	frame.data[200] = 0x5A;
	frame.data[319] = 0x5A;
	for(int i = 0; i < frame.dataSize; ++i) {
	   printf("%02X ",frame.data[i]);
	}
	printf("\n");
	DataPacket packet;
	packet.bufSize = 512;
	packet.buf = (uint8_t*)calloc(1,packet.bufSize);
	uint16_t len = 100;
    DataFrame rcvframe;
	memset(&rcvframe,0,sizeof(rcvframe));
	StcComCoder::getInstance()->encode(&frame,&packet);
	printf("packet len %d,data is\n",packet.bufSize);
	int i;
	for(i = 0; i < packet.bufSize; ++i) {
	    printf("%02X ",packet.buf[i]);
	}
	printf("\n");
#if 1
	int fd = -1;
	fd = open(COM_PATH,O_RDWR | O_NOCTTY);
	if(fd < 0) {
	   printf("open faild\n");
	   return -1;
	}
	printf("open %s success,fd %d\n",COM_PATH,fd);
	struct termios oldtio,newtio;
	tcgetattr(fd,&newtio);
	cfmakeraw(&newtio);
	printf("B9600 %d\n",B9600);
	printf("before set speed,c_iflag %X,c_oflag %X,c_lflag %X,c_cflag %X\n",newtio.c_iflag,newtio.c_oflag,newtio.c_lflag,newtio.c_cflag);
	cfsetospeed(&newtio,B9600);
    cfsetispeed(&newtio,B9600);
	printf("speed %d,%d\n",cfgetispeed(&newtio),cfgetospeed(&newtio));
	printf("after set speed,c_iflag %X,c_oflag %X,c_lflag %X,c_cflag %X\n",newtio.c_iflag,newtio.c_oflag,newtio.c_lflag,newtio.c_cflag);
	tcsetattr(fd,TCSANOW,&newtio);
	tcflush(fd,TCIOFLUSH);
	int ret,total = 0;
	uint8_t* data = packet.buf;
	uint16_t datalen = packet.bufSize;
	write(fd,data,5);
	sleep(1);
	while(1) {
	write(fd,data,5);
	sleep(1);
	write(fd,data + 5,datalen - 5);
	sleep(1);
	}
#if 0
	while(datalen != 0) {
		ret = write(fd,data + total,datalen);
		total += ret;
		datalen -= ret;
	}
#endif
	printf("write success,size %d\n",total);
    close(fd);
#else
	StcComCoder::getInstance()->decode(&packet,&rcvframe);
	printf("rcv frame:,rcvframe.dataSize %d\n",rcvframe.dataSize);
	printf("%02X ",rcvframe.ct);
	for(int i = 0; i < ST_DATA_LEN; ++i) {
       printf("%02X ",rcvframe.stData[i]);
	}
	for(int i = 0; i < rcvframe.dataSize; ++i) {
	   printf("%02X ",rcvframe.data[i]);
	}
	printf("\n");
#endif
	return 0;
}
