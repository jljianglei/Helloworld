#include "stc_com_coder.h"
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
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
	frame.dataSize = 4;
	frame.data = (uint8_t*)calloc(1,frame.dataSize);
	for(int i = 0; i < frame.dataSize; ++i) {
	   printf("%02X ",frame.data[i]);
	}
	printf("\n");
	DataPacket packet;
	packet.bufSize = 256;
	packet.buf = (uint8_t*)calloc(1,packet.bufSize);
	uint16_t len = 100;
	uint8_t* buf = (uint8_t*)calloc(1,len);
    StcComCoder::getInstance()->encode(&frame,&packet);
	StcComCoder::getInstance()->decode(&packet,buf);
	return 0;
}
