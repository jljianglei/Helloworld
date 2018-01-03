#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct {
    char * buf;
    size_t length;
}buffPacket;
static void callback(void * arg) {
    if(arg == NULL) {
       printf("arg is null\n");
       return;
    }
    buffPacket* packet = (buffPacket*)arg;
    printf("str length = %d,value is %s\n",packet->length,packet->buf);
    return;
}
void (*func(void * arg))(void *arg) {
    return &callback;
}
int main() {
    void (*pCallback)(void *);
    pCallback = func(NULL);
    buffPacket packet;
    packet.length = 100;
    packet.buf = calloc(1,packet.length);
    strncpy(packet.buf,"-------------------",packet.length - 1);
    packet.buf[packet.length] = '\0';
    if(pCallback) {
       (*pCallback)(&packet);
    }
    if(packet.buf) {
       free(packet.buf);
    }
    return 0;
}
