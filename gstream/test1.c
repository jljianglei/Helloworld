#include<stdio.h>
#include<gst/gst.h>
#include<stdlib.h>
#include<string.h>
int main(int argc,char** argv) {
    GstElement *pipeline,*filesrc,*decoder,*audiosink;
	gst_init(&argc,&argv);
	if(argc != 2) {
	   printf("usage: %s <mp3 filename>\n",argv[0]);
	   return 1;
	}
	return 0;
}
