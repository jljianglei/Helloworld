#include "stc_userial.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/prctl.h>
static int signal_fds[2] = {0,1};
static inline int create_signal_fds(fd_set* set) {
     if(signal_fds[0]==0 && socketpair(AF_UNIX, SOCK_STREAM, 0, signal_fds)<0) {
	    printf("create_signal_sockets:socketpair failed, errno: %d\n",errno);
		return -1;
	 }
	 FD_SET(signal_fds[0], set);
	 return signal_fds[0];
}

int send_wakeup_signal(char sig_cmd) {
    return send(signal_fds[1], &sig_cmd, sizeof(sig_cmd), 0);
}

static inline char reset_signal() {
    char sig_recv = -1;
	recv(signal_fds[0], &sig_recv, sizeof(sig_recv), MSG_WAITALL);
	return sig_recv;
}

static inline int is_signaled(fd_set* set) {
    return FD_ISSET(signal_fds[0], set);
}


int select_read(int fd,uint8_t* buf,uint16_t len) {
    fd_set input;
	int n = 0, ret = -1;
	uint8_t userial_running = 1;
	char reason = 0;
	struct timeval val;
	val.tv_sec = 0;
	val.tv_usec = 0;
	while (userial_running) {
	     int fd_max;
		 FD_ZERO(&input);
		 FD_SET(fd, &input);
		 fd_max = create_signal_fds(&input);
		 fd_max = fd_max > fd ? fd_max : fd;
		 n = select(fd_max+1, &input, NULL, NULL,NULL);
		 printf("n = %d\n",n);
		 if(is_signaled(&input)) {
		   reason = reset_signal();
		   if (reason == STC_USERIAL_RX_EXIT) {
		      printf("RX termination\n");
			  return -1;
		   }
		 }
		 if(n > 0) {
		   if (FD_ISSET(fd, &input)) {
		      ret = read(fd,buf,len);
			  printf("read ret = %d\n",ret);
			  if(0 == ret)
				  printf("read() return 0\n");
			  return ret;
		   }
		 }
		 else if (n < 0)
			  printf("select falid\n");
		 else if(n == 0)
			  printf("got a select TIMEOUT\n");
	}
	return ret;
}
uint16_t userial_write(int fd,uint8_t* data,uint16_t len) {
    int ret, total = 0;
	while(len != 0) {
	     ret = write(fd, data+total, len);
		 total += ret;
		 len -= ret;
	}
	return ((uint16_t)total);
}
