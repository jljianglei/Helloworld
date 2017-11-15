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

#define STC_DATA_CT_POS 0
#define STC_DATA_CT_NUM 1
#define STC_DATA_ST_POS 1
#define STC_DATA_POS 7
#define STC_CS_NUM 1
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
	 uint16_t setSize = 0;
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
	 printf("setSize %d\n",setSize);
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
	 printf("setSize %d\n",setSize);

	 //data setting
	 printf("frame->dataSize %d\n",frame->dataSize);
	 for(i = 0; i < frame->dataSize; ++i) {
	    packet->buf[setSize] = frame->data[i];
		setSize++;
		bycs = bycs ^ frame->data[i];
		if(frame->data[i] == ESC) {
		  packet->buf[setSize] = ESC;
		  setSize++;
		}
	 }
	 printf("setSize %d\n",setSize);

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
#if 0
	 printf("pakcet content is\n");
	 for(int i = 0; i < setSize; ++i) {
	    printf("%02X ",packet->buf[i]);
	 }
	 printf("\n");
#endif
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
void StcComCoder::decode(DataPacket* packet,DataFrame* dataframe) {
     cout << "StcComCoder::decode" << endl;
	 int i;
	 int setSize = 0;
	 uint8_t * frame = (uint8_t*)calloc(1,320);
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
#if 0
					  for(int j = 0; j < setSize - STC_CS_NUM; j++)
						  printf("%02X ",frame[j]);
					  printf("\n");
					  printf("decode frame is:\n");
#endif
					  memset(dataframe,0,sizeof(*dataframe));
                      dataframe->ct = frame[STC_DATA_CT_POS];
					  memcpy(dataframe->stData,frame + STC_DATA_ST_POS,ST_DATA_LEN);
					  dataframe->dataSize = setSize - (STC_DATA_CT_NUM + ST_DATA_LEN + STC_CS_NUM);
					  dataframe->data = (uint8_t*)calloc(1,dataframe->dataSize);
					  memcpy(dataframe->data,frame + STC_DATA_POS,dataframe->dataSize);
#if 0
					  printf("%02X ",dataframe->ct);
					  int j;
					  for(j = 0; j < ST_DATA_LEN; ++j) {
					      printf("%02X ",dataframe->stData[j]);
					  }
					  for(j = 0; j < dataframe->dataSize; ++j) {
					      printf("%02X ",dataframe->data[j]);
					  }
					  printf("\n");
#endif
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
