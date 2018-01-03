#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
typedef struct{
    char str[100];
    int data;
}example;
enum opcodeType {
     INT_OPCODE,
     CHAR_OPCODE,
     STRUCT_OPCODE,
     MAX_OPCODE
};
static void * callback_func(int opcode,void * arg) {
   int * p;
   const char * str;
   example* exa;
    switch(opcode) {
      case INT_OPCODE:
          p = (int*)arg;
          printf("*p = %d\n",*p);
          break;
      case CHAR_OPCODE:
          str = (const char*)arg;
          printf("%s\n",str);
          break;
      case STRUCT_OPCODE:
          exa = (example*)arg;
          printf("%s,%d\n",exa->str,exa->data);
          break;
      case MAX_OPCODE:
          printf("invalid opcode\n");
          break;
      default:
          break;
   }
   return NULL;
}
int main() {
    int a = 10;
    callback_func(INT_OPCODE,&a);
    const char * str = "hello";
    callback_func(CHAR_OPCODE,(void*)str);
    example exa;
    strncpy(exa.str,"nihao",100);
    exa.data = 1;
    callback_func(STRUCT_OPCODE,&exa);
    callback_func(MAX_OPCODE,NULL);
    return 0;
}

