#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<error.h>
#include<errno.h>
#define MAX_BUF_SIZE 30
int main() {
    int s[2];
    int w,r;
    const char * string = "this is a test string";
    char * buf = (char*)calloc(1,MAX_BUF_SIZE);
    if(socketpair(AF_UNIX,SOCK_STREAM,0,s) == -1) {
      printf("create unname socket pair faild %s\n",strerror(errno));
      exit(-1);
    }
    if(w = write(s[0],string,strlen(string)) == -1) {
      printf("write to s[0] faild %s\n",strerror(errno));
      exit(-1);
    }
    if(r = read(s[1],buf,MAX_BUF_SIZE) == -1) {
      printf("read from s[1] faild %s\n",strerror(errno));
      exit(-1);
    }
    printf("read buf,%s\n",buf);
    return 0;
}
