#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void func2(void *arg) {
   printf("%s,%d\n",__func__,__LINE__);
}
typedef struct {
   void (*callbackTest)(void *);
   char * str;
   size_t length;
}example;
void * func(void * arg) {
   example * pe = (example*)arg;
   pe->callbackTest(NULL);
   printf("file path is %s\n",pe->str);
   return NULL;
}
int main() {
   example e1;
   e1.callbackTest = &func2;
   e1.length = 100;
   e1.str = calloc(1,e1.length);
   if(e1.str == NULL) {
      printf("calloc faild\n");
      return -1;
   }
   strncpy(e1.str,"record.txt",e1.length - 1);
   e1.str[e1.length - 1] = '\0';
   func(&e1);
   return 0;
}
