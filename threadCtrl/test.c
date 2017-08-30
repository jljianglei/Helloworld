#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
#include<unistd.h>
#include<errno.h>
enum {
   EXIT_THREAD
};
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

void * loop(void *msg) {
	 fd_set input;
	 int fd;
	 char reason = -1;
	 struct timeval val;
	 val.tv_sec = 0;
	 val.tv_usec = 0;
     while(1) {
		  printf("looping\n");
	      FD_ZERO(&input);
		  fd = create_signal_fds(&input);
          select(fd+1, &input, NULL,NULL,&val);
		  if(is_signaled(&input)) {
		    reason = reset_signal();
			if(reason == EXIT_THREAD)
				break;
		  }
	 }
	 printf("exit loop\n");
}
int main() {
    pthread_t pid = 0;
	pthread_create(&pid,NULL,loop,NULL);
    usleep(100);
	send_wakeup_signal(EXIT_THREAD);
	sleep(1);
	return 0;
}
