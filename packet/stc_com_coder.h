#ifndef _STC_COM_CODER_
#define _STC_COM_CODER_
#define ST_DATA_LEN 6
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef struct {
     uint8_t ct;
	 uint8_t stData[ST_DATA_LEN];
	 uint8_t* data;
     uint16_t dataSize;
}DataFrame;
typedef struct {
	uint8_t* buf;
	uint16_t bufSize;
}DataPacket;
class StcComCoder {
    private:
		static StcComCoder* mInstance;
		StcComCoder();
		StcComCoder(const StcComCoder& coder);
		StcComCoder& operator=(const StcComCoder& code);
		bool checkSum(uint8_t* buf,uint16_t size);
	public:
		static StcComCoder* getInstance();
		void encode(DataFrame* frame,DataPacket* packet);
		void decode(DataPacket* packet,uint8_t* frame);

};
#endif
