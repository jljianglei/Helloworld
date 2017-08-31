#include "stc_com_coder.h"
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
const uint8_t ESC = 0x5A;
const uint8_t SoT = 0x02;
const uint8_t EoT = 0x03;

const uint8_t STATUS_WAIT_ESC = 0x00;
const uint8_t STATUS_WAIT_STX = 0x01;
const uint8_t STATUS_WAIT_DATA = 0x02;
const uint8_t STATUS_WAIT_ETX = 0x03;
static uint8_t rcvStatus = STATUS_WAIT_ESC;
StcComCoder* StcComCoder::mInstance = NULL;
StcComCoder* StcComCoder::getInstance() {
    if(mInstance == NULL) {
	  mInstance = new StcComCoder;
	}
	return mInstance;
}
StcComCoder::StcComCoder() {
}
void StcComCoder::encode(DataFrame* frame,DataPacket* packet) {
     cout << "StcComCoder::encode" << endl;
	 uint8_t setSize = 0;
	 uint8_t bycs = 0;
	 int i;
     //esc setting
	 packet->buf[setSize] = ESC;
	 setSize++;
	 //SoT setting
	 packet->buf[setSize] = SoT;
	 setSize++;
	 //CT setting
	 packet->buf[setSize] = frame->ct;
	 setSize++;
	 bycs = bycs ^ frame->ct;
	 if(frame->ct == ESC) {
	    packet->buf[setSize] = ESC;
		setSize++;
	 }
	 //ST setting
     for(i = 0; i < ST_DATA_LEN; ++i) {
	    packet->buf[setSize] = frame->stData[i];
		setSize++;
		bycs = bycs ^ frame->stData[i];
		if(frame->stData[i] == ESC) {
		  packet->buf[setSize] = ESC;
		  setSize++;
		}
	 }

	 //data setting
	 for(i = 0; i < frame->dataSize; ++i) {
	    packet->buf[setSize] = frame->data[i];
		setSize++;
		bycs = bycs ^ frame->data[i];
		if(frame->data[i] == ESC) {
		  packet->buf[setSize] = ESC;
		  setSize++;
		}
	 }

	 //cs setting
	 packet->buf[setSize] = bycs;
	 setSize++;
	 if(bycs == ESC) {
	   packet->buf[setSize] = ESC;
	   setSize++;
	 }

	 //esc setting
	 packet->buf[setSize] = ESC;
	 setSize++;
	 //EoT setting
	 packet->buf[setSize] = EoT;
	 setSize++;
	 packet->bufSize = setSize;
	 printf("pakcet content is\n");
	 for(int i = 0; i < setSize; ++i) {
	    printf("%02X ",packet->buf[i]);
	 }
	 printf("\n");
}
bool StcComCoder::checkSum(uint8_t* buf,uint16_t size) {
     uint8_t checkbysc = 0;
	 uint8_t checknum = size - 1;
	 uint8_t packetBysc = buf[checknum];
	 printf("checkSum,packetbysc %02X\n",packetBysc);
	 int i;
	 for(i = 0; i < checknum; ++i) {
	    checkbysc = checkbysc ^ buf[i];
	 }
	 printf("checkbysc %02X\n",checkbysc);
	 if(checkbysc == packetBysc)
		 return true;
	 else
		 return false;
}
void StcComCoder::decode(DataPacket* packet,uint8_t* frame) {
     cout << "StcComCoder::decode" << endl;
	 int i;
	 int setSize = 0;
	 for(i = 0; i < packet->bufSize; ++i) {
        switch(rcvStatus) {
			//wait esc(0x5A),change wait state to SoT,do not store data
		    case STATUS_WAIT_ESC:
				if(packet->buf[i] == ESC) {
				  rcvStatus = STATUS_WAIT_STX;
				}
				break;
			//wait SoT(0x02)
			case STATUS_WAIT_STX:
				//if wait SoT(0x02),change wait state to data,do not store data
				if(packet->buf[i] == SoT) {
				   rcvStatus = STATUS_WAIT_DATA;
				}
				//if not SoT(0x02),return to wait ESC,reset data
				else {
				   rcvStatus = STATUS_WAIT_ESC;
				   memset(frame,0,sizeof(frame));
				}
				break;
			case STATUS_WAIT_DATA:
				//if ESC(0x5A),change state to EoT,do not store data
				if(packet->buf[i] == ESC) {
				   rcvStatus = STATUS_WAIT_ETX;
				}
				//store data
				else {
				   frame[setSize] = packet->buf[i];
				   setSize++;
				}
				break;
			case STATUS_WAIT_ETX:
				 //if(0x5A)(0x5A),it is treat as data byte
                 if(packet->buf[i] == ESC) {
				   //double ESC,save one in decode data,change state to data
                   frame[setSize] = packet->buf[i];
				   setSize++;
				   rcvStatus = STATUS_WAIT_DATA;
				 }
				 //if(0x5A)(0x02),wait end error,change state to wait data,reset frame
				 else if(packet->buf[i] == SoT) {
				     rcvStatus = STATUS_WAIT_DATA;
					 memset(frame,0,100);
					 setSize = 0;
				 }
				 //if(0x5A)(0x03),wait end ok
				 else if(packet->buf[i] == EoT) {
                      if(checkSum(frame,setSize)) {
					    printf("decode success\n");
					  }
					  rcvStatus = STATUS_WAIT_ESC;
					  for(int j = 0; j < setSize - 1; j++)
						  printf("%02X ",frame[j]);
					  printf("\n");
					  return;
				 }
				 else {
				    rcvStatus = STATUS_WAIT_ESC;
					memset(frame,0,100);
					setSize = 0;
				 }
				 break;
			default:
				    rcvStatus = STATUS_WAIT_ESC;
					memset(frame,0,100);
					setSize = 0;
					break;

		}
	 }
}
