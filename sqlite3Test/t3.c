#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef int (*funcCB)(void *);
typedef struct {
   char * str;
   int size;
}buffPacket;
int func(void * arg) {
  buffPacket* buf = ( buffPacket*)arg;
  if(buf == NULL) {
     printf("arg is null\n");
     return -1;
  }
  printf("str size is %d,value is %s\n",buf->size,buf->str);
  return 0;
}
funcCB globalPoint;
void registerCallback(int (*cb)(void*),void * arg) {
     globalPoint = cb;
     (*globalPoint)(arg);
}
int main() {
    buffPacket p1;
    printf("sizeof(buffPacket) = %d\n",sizeof(buffPacket));
    p1.size = 100;
    p1.str = calloc(1,p1.size);
    strncpy(p1.str,"buffPacket test",p1.size - 1);
    p1.str[p1.size] = '\0';
    registerCallback(&func,&p1);
    if(p1.str) {
      free(p1.str);
      p1.str = NULL;
      p1.size = 0;
    }
    return 0;
}

